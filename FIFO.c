/*
 * fifo_trace.c
 *
 * POSIX C (C99) implementation of FIFO page replacement with a verbose trace table.
 *
 * Compile:
 *   gcc -std=c99 -O2 -Wall -o fifo_trace fifo_trace.c
 *
 * Usage:
 *   ./fifo_trace
 *   ./fifo_trace 4
 *   ./fifo_trace 3 "1,2,3,4,2,1,5,6,2,1,2,3,7,6,3,2,1,2,3,6"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_REF 2048
#define MAX_FRAMES 64

/* Parse comma-separated integers into refs[]; returns count */
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

/* Print table header for the trace */
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

/* FIFO trace simulator */
static int simulate_fifo_trace(const int refs[], int n, int frames) {
    int frames_arr[MAX_FRAMES];
    if (frames > MAX_FRAMES) frames = MAX_FRAMES;
    for (int i = 0; i < frames; ++i) frames_arr[i] = INT_MIN;

    int next = 0;    /* index to insert next (circular) */
    int faults = 0;
    int filled = 0;  /* how many frames are filled */

    printf("\n=== FIFO TRACE ===\n");
    print_header(frames);

    for (int i = 0; i < n; ++i) {
        int page = refs[i];
        int hit = 0;
        for (int j = 0; j < frames; ++j) {
            if (frames_arr[j] == page) { hit = 1; break; }
        }

        if (hit) {
            /* HIT: no fault, no eviction */
            print_row(i, page, frames_arr, frames, "HIT", INT_MIN, faults);
            continue;
        }

        /* MISS */
        faults++;
        int evicted = INT_MIN;
        if (filled < frames) {
            /* there's an empty slot at index `next` (we maintain invariant that
               next points to the next free/replace slot). Place page there. */
            frames_arr[next] = page;
            filled++;
            /* evicted remains INT_MIN */
            next = (next + 1) % frames;
        } else {
            /* evict the page at 'next' (FIFO order) */
            evicted = frames_arr[next];
            frames_arr[next] = page;
            next = (next + 1) % frames;
        }
        print_row(i, page, frames_arr, frames, "MISS", evicted, faults);
    }

    print_footer(frames);
    printf("FIFO total faults = %d\n", faults);
    return faults;
}

int main(int argc, char **argv) {
    int frames = 3;
    int refs[MAX_REF];
    int nrefs = 0;
    const char *default_ref_str = "1,2,3,4,2,1,5,6,2,1,2,3,7,6,3,2,1,2,3,6";

    /* parse frames if provided */
    if (argc >= 2) {
        frames = atoi(argv[1]);
        if (frames <= 0 || frames > MAX_FRAMES) {
            fprintf(stderr, "Invalid frames count. Using default frames = 3 (max %d).\n", MAX_FRAMES);
            frames = 3;
        }
    }

    /* parse reference string if provided */
    if (argc >= 3) {
        nrefs = parse_refs(argv[2], refs, MAX_REF);
        if (nrefs == 0) {
            fprintf(stderr, "Couldn't parse provided ref string. Using default.\n");
            nrefs = parse_refs(default_ref_str, refs, MAX_REF);
        }
    } else {
        nrefs = parse_refs(default_ref_str, refs, MAX_REF);
    }

    if (nrefs == 0) {
        fprintf(stderr, "No references to process. Exiting.\n");
        return 1;
    }

    printf("\nRunning FIFO page replacement trace\n");
    printf("Reference string (%d refs): ", nrefs);
    for (int i = 0; i < nrefs; ++i) {
        if (i) putchar(',');
        printf("%d", refs[i]);
    }
    printf("\nFrames = %d\n", frames);

    simulate_fifo_trace(refs, nrefs, frames);

    /* example address calc (from earlier lab) */
    int logical = 2700, page_size = 1024;
    int page_number = logical / page_size;
    int offset = logical - page_number * page_size;
    printf("\nExample: Logical address = %d, page size = %d bytes -> page = %d, offset = %d\n",
           logical, page_size, page_number, offset);

    return 0;
}
