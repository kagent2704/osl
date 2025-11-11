/* 
 * Program: FCFS CPU Scheduling Algorithm
 * Author: Kashmira Patil
 *
 * Description:
 * This program simulates the First Come First Serve (FCFS) scheduling algorithm.
 * Processes are scheduled in the order they arrive.
 */

#include <stdio.h>

int main() {
    int n, i;
    float avg_wait = 0, avg_tat = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    int bt[n], wt[n], tat[n];

    printf("Enter burst time for each process:\n");
    for (i = 0; i < n; i++) {
        printf("P%d: ", i + 1);
        scanf("%d", &bt[i]);
    }

    wt[0] = 0;  // waiting time for first process is 0

    // Calculate waiting time
    for (i = 1; i < n; i++)
        wt[i] = bt[i - 1] + wt[i - 1];

    // Calculate turnaround time
    for (i = 0; i < n; i++)
        tat[i] = bt[i] + wt[i];

    // Calculate averages
    for (i = 0; i < n; i++) {
        avg_wait += wt[i];
        avg_tat += tat[i];
    }

    avg_wait /= n;
    avg_tat /= n;

    // Display results
    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (i = 0; i < n; i++)
        printf("P%d\t\t%d\t\t%d\t\t%d\n", i + 1, bt[i], wt[i], tat[i]);

    printf("\nAverage Waiting Time: %.2f", avg_wait);
    printf("\nAverage Turnaround Time: %.2f\n", avg_tat);

    return 0;
}
