/*
 * lru_trace.c
 * POSIX C (C99). Verbose tabular trace for LRU page replacement only.
 *
 * Compile:
 *   gcc -std=c99 -O2 -Wall -o lru_trace lru_trace.c
 *
 * Usage:
 *   ./lru_trace                # default frames = 3, default reference string
 *   ./lru_trace 4              # frames = 4, default reference string
 *   ./lru_trace 3 "1,2,3,4,2,1,5,6,..."  # frames = 3 and custom ref string
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_REF 2048
#define MAX_FRAMES 32

/* Parse comma-separated numbers into refs[]; returns count */
static int parse_refs(const char *s, int refs[], int maxlen) {
    if (!s) return 0;
    int count = 0;
    const char *p = s;
    while (*p && count < maxlen) {
        while (*p == ' ' || *p == '\t') p++;
        char *end;
        long val = strtol(p, &end, 10);
        if (p == end) break;
        refs[count++] = (int)val;
        p = end;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == ',') p++;
    }
    return count;
}

/* Print table header for LRU trace */
static void print_header(int frames) {
    printf("+-----+-----+");
    for (int f = 0; f < frames; ++f) printf("--------+");
    printf("--------+---------+--------+\n");
    printf("| idx | ref |");
    for (int f = 0; f < frames; ++f) printf(" frame%-2d |", f);
    printf(" action | evicted | faults |\n");
    printf("+-----+-----+");
    for (int f = 0; f < frames; ++f) printf("--------+");
    printf("--------+---------+--------+\n");
}

/* Print one row */
static void print_row(int idx, int ref, int frames_arr[], int frames_count, const char *action, int evicted, int faults) {
    printf("|%4d |%4d |", idx+1, ref);
    for (int f = 0; f < frames_count; ++f) {
        if (frames_arr[f] == INT_MIN) printf("   -    |");
        else printf("   %2d   |", frames_arr[f]);
    }
    printf(" %6s |", action);
    if (evicted == INT_MIN) printf("    -    |");
    else printf("   %2d    |", evicted);
    printf("  %4d |\n", faults);
}

/* Print footer divider */
static void print_footer(int frames) {
    printf("+-----+-----+");
    for (int f = 0; f < frames; ++f) printf("--------+");
    printf("--------+---------+--------+\n");
}

/* LRU verbose trace implementation:
 * frames_arr stores pages in order [LRU ... MRU]
 */
static int simulate_lru_verbose(const int refs[], int n, int frames) {
    int frames_arr[MAX_FRAMES];
    for (int i = 0; i < frames; ++i) frames_arr[i] = INT_MIN;
    int used = 0; /* number of frames filled (MRU index is used-1) */
    int faults = 0;

    printf("\n=== LRU TRACE ===\n");
    print_header(frames);

    for (int i = 0; i < n; ++i) {
        int r = refs[i];
        int pos = -1;
        /* find if r is in frames */
        for (int j = 0; j < used; ++j) {
            if (frames_arr[j] == r) { pos = j; break; }
        }

        if (pos != -1) {
            /* HIT: move this entry to MRU (end) */
            int temp = frames_arr[pos];
            for (int k = pos; k < used - 1; ++k) frames_arr[k] = frames_arr[k+1];
            frames_arr[used - 1] = temp;
            print_row(i, r, frames_arr, frames, "HIT", INT_MIN, faults);
        } else {
            /* MISS */
            faults++;
            int evicted = INT_MIN;
            if (used < frames) {
                /* place at MRU position */
                frames_arr[used++] = r;
            } else {
                /* evict LRU (index 0) */
                evicted = frames_arr[0];
                for (int k = 0; k < frames - 1; ++k) frames_arr[k] = frames_arr[k+1];
                frames_arr[frames - 1] = r;
            }
            print_row(i, r, frames_arr, frames, "MISS", evicted, faults);
        }
    }

    print_footer(frames);
    printf("LRU total faults = %d\n", faults);
    return faults;
}

int main(int argc, char **argv) {
    int frames = 3;
    int refs[MAX_REF];
    int nrefs;

    const char *default_ref_str = "1,2,3,4,2,1,5,6,2,1,2,3,7,6,3,2,1,2,3,6";

    if (argc >= 2) {
        frames = atoi(argv[1]);
        if (frames <= 0 || frames > MAX_FRAMES) {
            fprintf(stderr, "Invalid frames count. Using default 3 (max %d allowed).\n", MAX_FRAMES);
            frames = 3;
        }
    }

    if (argc >= 3) {
        nrefs = parse_refs(argv[2], refs, MAX_REF);
        if (nrefs == 0) {
            fprintf(stderr, "Couldn't parse provided ref string. Using default.\n");
            nrefs = parse_refs(default_ref_str, refs, MAX_REF);
        }
    } else {
        nrefs = parse_refs(default_ref_str, refs, MAX_REF);
    }

    printf("\nRunning LRU page replacement trace\n");
    printf("Reference string (%d refs): ", nrefs);
    for (int i = 0; i < nrefs; ++i) {
        if (i) putchar(',');
        printf("%d", refs[i]);
    }
    printf("\nFrames = %d\n", frames);

    simulate_lru_verbose(refs, nrefs, frames);

    /* example page / offset calculation (kept from original lab) */
    int logical = 2700, page_size = 1024;
    int page_number = logical / page_size;
    int offset = logical - page_number * page_size;
    printf("\nExample: Logical address = %d, page size = %d bytes -> page = %d, offset = %d\n",
           logical, page_size, page_number, offset);

    return 0;
}
