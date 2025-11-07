/*
 * srtf.c
 * ======
 * Implementation of CPU Scheduling Algorithm: Shortest Job First (Preemptive)
 *
 * This program simulates the SRTF (Shortest Remaining Time First) algorithm,
 * which is the preemptive version of Shortest Job First.
 *
 * Key Concepts:
 * - Preemptive: A running process can be interrupted if a new process
 *   arrives with a shorter remaining burst time.
 * - The ready queue is always searched for the process with the least
 *   remaining time to execute next.
 *
 * Performance Metrics Calculated:
 * - Completion Time (CT): The time at which a process finishes execution.
 * - Turnaround Time (TAT): CT - Arrival Time.
 * - Waiting Time (WT): TAT - Burst Time.
 */

#include <stdio.h>

int main() {
    int n, i;
    printf("Enter the number of processes (<= 20): ");
    scanf("%d", &n);
    if (n <= 0 || n > 20) { printf("Invalid n\n"); return 0; }

    int at[20], bt[20], rem[20], temp[20];
    int ct[20], wt[20], tat[20];
    int time = 0, count = 0;
    float avg_wt = 0, avg_tat = 0;

    printf("\nEnter Arrival Time and Burst Time for each process:\n");
    for (i = 0; i < n; i++) {
        printf("P%d Arrival Time: ", i + 1);
        scanf("%d", &at[i]);
        printf("P%d Burst Time: ", i + 1);
        scanf("%d", &bt[i]);
        if (bt[i] < 0 || at[i] < 0) { printf("Invalid input\n"); return 0; }
        temp[i] = bt[i];
        rem[i] = bt[i]; // remaining time
    }

    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");
    time = 0;
    count = 0;

    while (count < n) {
        int smallest = -1;
        // find process with smallest remaining time among arrived and unfinished
        for (i = 0; i < n; i++) {
            if (at[i] <= time && rem[i] > 0) {
                if (smallest == -1 || rem[i] < rem[smallest]) {
                    smallest = i;
                }
            }
        }

        if (smallest == -1) {
            // No process is ready at this time, CPU idle -> advance time
            time++;
            continue;
        }

        // execute smallest for 1 unit
        rem[smallest]--;
        time++;

        if (rem[smallest] == 0) {
            count++;
            ct[smallest] = time; // finished at current time
            tat[smallest] = ct[smallest] - at[smallest];
            wt[smallest] = tat[smallest] - temp[smallest];
            avg_wt += wt[smallest];
            avg_tat += tat[smallest];
        }
    }

    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", i + 1, at[i], temp[i], ct[i], tat[i], wt[i]);
    }
    printf("\nAverage Waiting Time = %.2f", avg_wt / n);
    printf("\nAverage Turnaround Time = %.2f\n", avg_tat / n);

    return 0;
}