/* banker.c
 * Banker’s Algorithm simulation (safety check + request test)
 * Compile: gcc -Wall -o banker banker.c
 * Run:     ./banker
 *
 * Author: for lab use
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n, m;
    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0) return 0;
    printf("Enter number of resource types: ");
    if (scanf("%d", &m) != 1 || m <= 0) return 0;

    // Allocate matrices/vectors
    int **alloc = malloc(n * sizeof(*alloc));
    int **max = malloc(n * sizeof(*max));
    int **need = malloc(n * sizeof(*need));
    int *avail = malloc(m * sizeof(*avail));
    for (int i = 0; i < n; ++i) {
        alloc[i] = malloc(m * sizeof(**alloc));
        max[i]   = malloc(m * sizeof(**max));
        need[i]  = malloc(m * sizeof(**need));
    }

    // Input Allocation matrix
    printf("\nEnter Allocation matrix (n x m):\n");
    for (int i = 0; i < n; ++i) {
        printf("P%d: ", i);
        for (int j = 0; j < m; ++j) scanf("%d", &alloc[i][j]);
    }

    // Input Max matrix
    printf("\nEnter Max matrix (n x m):\n");
    for (int i = 0; i < n; ++i) {
        printf("P%d: ", i);
        for (int j = 0; j < m; ++j) scanf("%d", &max[i][j]);
    }

    // Input Available vector
    printf("\nEnter Available vector (m):\n");
    for (int j = 0; j < m; ++j) scanf("%d", &avail[j]);

    // Compute Need = Max - Allocation
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) need[i][j] = max[i][j] - alloc[i][j];

    // Safety algorithm
    int *work = malloc(m * sizeof(*work));
    int *finish = calloc(n, sizeof(*finish));
    int *safeSeq = malloc(n * sizeof(*safeSeq));
    for (int j = 0; j < m; ++j) work[j] = avail[j];

    int count = 0;
    while (count < n) {
        int found = 0;
        for (int i = 0; i < n; ++i) {
            if (!finish[i]) {
                int can = 1;
                for (int j = 0; j < m; ++j)
                    if (need[i][j] > work[j]) { can = 0; break; }
                if (can) {
                    // pretend to allocate and finish process i
                    for (int j = 0; j < m; ++j) work[j] += alloc[i][j];
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) break; // no suitable process found -> unsafe
    }

    if (count == n) {
        printf("\nSystem is in a SAFE state.\nSafe sequence: ");
        for (int i = 0; i < n; ++i) {
            if (i) printf(" -> ");
            printf("P%d", safeSeq[i]);
        }
        printf("\n");
    } else {
        printf("\nSystem is in an UNSAFE state (no safe sequence found).\n");
    }

    // Offer to test an additional request
    char choice;
    printf("\nDo you want to test a resource request? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        int pid;
        printf("Enter process id (0 to %d) making the request: ", n-1);
        scanf("%d", &pid);
        if (pid < 0 || pid >= n) {
            printf("Invalid PID.\n");
        } else {
            int *req = malloc(m * sizeof(*req));
            printf("Enter request vector (m):\n");
            for (int j = 0; j < m; ++j) scanf("%d", &req[j]);

            // Check request <= need
            int ok = 1;
            for (int j = 0; j < m; ++j)
                if (req[j] > need[pid][j]) { ok = 0; break; }
            if (!ok) {
                printf("Error: Process has exceeded its maximum claim (request > need).\n");
            } else {
                // Check request <= available
                ok = 1;
                for (int j = 0; j < m; ++j)
                    if (req[j] > avail[j]) { ok = 0; break; }
                if (!ok) {
                    printf("Request cannot be granted immediately (not enough available resources).\n");
                } else {
                    // Try to allocate temporarily and run safety check
                    // create copies
                    int *work2 = malloc(m * sizeof(*work2));
                    int *finish2 = calloc(n, sizeof(*finish2));
                    int **alloc2 = malloc(n * sizeof(*alloc2));
                    int **need2  = malloc(n * sizeof(*need2));
                    for (int i = 0; i < n; ++i) {
                        alloc2[i] = malloc(m * sizeof(**alloc2));
                        need2[i]  = malloc(m * sizeof(**need2));
                        for (int j = 0; j < m; ++j) {
                            alloc2[i][j] = alloc[i][j];
                            need2[i][j]  = need[i][j];
                        }
                    }
                    for (int j = 0; j < m; ++j) work2[j] = avail[j];

                    // pretend allocate
                    for (int j = 0; j < m; ++j) {
                        alloc2[pid][j] += req[j];
                        need2[pid][j]  -= req[j];
                        work2[j]       -= req[j];
                    }

                    // safety on modified state
                    int cnt2 = 0;
                    int *safe2 = malloc(n * sizeof(*safe2));
                    while (cnt2 < n) {
                        int found2 = 0;
                        for (int i = 0; i < n; ++i) {
                            if (!finish2[i]) {
                                int can2 = 1;
                                for (int j = 0; j < m; ++j)
                                    if (need2[i][j] > work2[j]) { can2 = 0; break; }
                                if (can2) {
                                    for (int j = 0; j < m; ++j) work2[j] += alloc2[i][j];
                                    safe2[cnt2++] = i;
                                    finish2[i] = 1;
                                    found2 = 1;
                                }
                            }
                        }
                        if (!found2) break;
                    }

                    if (cnt2 == n) {
                        printf("Request CAN be granted safely.\nNew safe sequence: ");
                        for (int i = 0; i < n; ++i) {
                            if (i) printf(" -> ");
                            printf("P%d", safe2[i]);
                        }
                        printf("\n");
                    } else {
                        printf("Request CANNOT be granted: it leads to an unsafe state.\n");
                    }

                    // free temporary copies
                    for (int i = 0; i < n; ++i) { free(alloc2[i]); free(need2[i]); }
                    free(alloc2); free(need2);
                    free(work2); free(finish2); free(safe2);
                }
            }
            free(req);
        }
    }

    // free all allocated memory
    for (int i = 0; i < n; ++i) {
        free(alloc[i]); free(max[i]); free(need[i]);
    }
    free(alloc); free(max); free(need);
    free(avail); free(work); free(finish); free(safeSeq);

    return 0;
}
