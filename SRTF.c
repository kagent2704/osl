/* srtf.c
 * Shortest Remaining Time First (SRTF) CPU Scheduling Simulation
 * Compile: gcc -Wall -o srtf srtf.c
 * Run:     ./srtf
 *
 * Author: for lab use
 */

#include <stdio.h>
#include <limits.h>

int main(void) {
    int n, i, completed = 0;
    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0) return 0;

    int at[n], bt[n], rem[n], ct[n], tat[n], wt[n], pid[n];
    for (i = 0; i < n; ++i) {
        pid[i] = i + 1;
    }

    printf("Enter Arrival Time and Burst Time for each process:\n");
    for (i = 0; i < n; ++i) {
        printf("P%d -> AT BT: ", i + 1);
        scanf("%d %d", &at[i], &bt[i]);
        rem[i] = bt[i];
        ct[i] = tat[i] = wt[i] = 0;
    }

    int time = 0;
    int min_index;
    int found;
    int first_arrival = INT_MAX;
    for (i = 0; i < n; ++i) if (at[i] < first_arrival) first_arrival = at[i];
    time = first_arrival; /* start from first arrival to avoid unnecessary idle counting */

    /* we'll store pid executed at each time unit to build Gantt */
    int timeline_capacity = 10000;
    int timeline[timeline_capacity];
    int tlen = 0;

    while (completed < n) {
        /* find process with minimum remaining time among arrived & not complete */
        min_index = -1;
        int min_rem = INT_MAX;
        found = 0;
        for (i = 0; i < n; ++i) {
            if (at[i] <= time && rem[i] > 0) {
                if (rem[i] < min_rem) {
                    min_rem = rem[i];
                    min_index = i;
                    found = 1;
                } else if (rem[i] == min_rem) {
                    /* Tie-breaker: choose process with earlier arrival, then smaller PID */
                    if (at[i] < at[min_index] || (at[i] == at[min_index] && pid[i] < pid[min_index])) {
                        min_index = i;
                    }
                }
            }
        }

        if (!found) {
            /* CPU idle for this time unit (no arrived process). advance time to next arrival */
            /* record idle with pid 0 */
            if (tlen < timeline_capacity) timeline[tlen++] = 0;
            /* jump to earliest next arrival */
            int next_arrival = INT_MAX;
            for (i = 0; i < n; ++i)
                if (rem[i] > 0 && at[i] > time && at[i] < next_arrival)
                    next_arrival = at[i];
            if (next_arrival == INT_MAX) break; /* nothing left */
            time = next_arrival;
            continue;
        }

        /* execute chosen process for 1 time unit */
        rem[min_index]--;
        if (tlen < timeline_capacity) timeline[tlen++] = pid[min_index];
        /* if process finished, record completion */
        if (rem[min_index] == 0) {
            completed++;
            ct[min_index] = time + 1; /* process completes at end of this time unit */
        }
        time++; /* advance time by 1 unit */
    }

    /* compute TAT and WT and averages */
    double avg_tat = 0.0, avg_wt = 0.0;
    for (i = 0; i < n; ++i) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        avg_tat += tat[i];
        avg_wt += wt[i];
    }
    avg_tat /= n;
    avg_wt /= n;

    /* Print Gantt chart (compress consecutive same PIDs) */
    printf("\nGantt Chart:\n");
    int idx = 0;
    int cur_pid, start_time = 0;
    int time_marker = 0;
    while (idx < tlen) {
        cur_pid = timeline[idx];
        start_time = time_marker;
        while (idx < tlen && timeline[idx] == cur_pid) { idx++; time_marker++; }
        int end_time = time_marker;
        if (cur_pid == 0)
            printf("| Idle (%d - %d) ", start_time, end_time);
        else
            printf("| P%d (%d - %d) ", cur_pid, start_time, end_time);
    }
    printf("|\n");

    /* Print table of results */
    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");
    for (i = 0; i < n; ++i) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], ct[i], tat[i], wt[i]);
    }

    printf("\nAverage Turnaround Time = %.2f\n", avg_tat);
    printf("Average Waiting Time    = %.2f\n", avg_wt);

    return 0;
}
