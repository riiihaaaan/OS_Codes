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
#include <stdlib.h>
#include <limits.h> // For INT_MAX

/* Process Control Block (PCB) Structure */
typedef struct {
    int id;              // Process identifier
    int arrival_time;    // Time process arrives
    int burst_time;      // Total CPU time required
    int remaining_time;  // Time still needed to complete
    int completion_time; // Time when process finishes
    int turnaround_time; // Total time in system
    int waiting_time;    // Time spent waiting
} Process;

void print_results(Process proc[], int n);

/**
 * @brief Main function to run the SRTF simulation.
 */
int main() {
    int n;

    printf("--- Shortest Job First (Preemptive) Scheduling ---\n");
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    if (n <= 0) {
        printf("Error: Must have at least one process.\n");
        return 1;
    }

    Process* processes = (Process*)malloc(n * sizeof(Process));

    printf("\nEnter process details (Arrival Time Burst Time):\n");
    for (int i = 0; i < n; i++) {
        printf("Process P%d: ", i);
        processes[i].id = i;
        scanf("%d %d", &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
    }

    int current_time = 0;
    int completed = 0;

    while (completed != n) {
        int shortest_idx = -1;
        int min_remaining = INT_MAX;

        // Find the process with the shortest remaining time that has arrived
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                if (processes[i].remaining_time < min_remaining) {
                    min_remaining = processes[i].remaining_time;
                    shortest_idx = i;
                }
            }
        }

        if (shortest_idx == -1) {
            current_time++; // CPU is idle
        } else {
            // Execute the shortest process for one time unit
            processes[shortest_idx].remaining_time--;
            current_time++;

            // If process finished, calculate metrics
            if (processes[shortest_idx].remaining_time == 0) {
                processes[shortest_idx].completion_time = current_time;
                processes[shortest_idx].turnaround_time = processes[shortest_idx].completion_time - processes[shortest_idx].arrival_time;
                processes[shortest_idx].waiting_time = processes[shortest_idx].turnaround_time - processes[shortest_idx].burst_time;
                completed++;
            }
        }
    }

    print_results(processes, n);
    free(processes);
    return 0;
}

/**
 * @brief Prints a formatted table of scheduling results.
 */
void print_results(Process proc[], int n) {
    float total_wt = 0, total_tat = 0;

    printf("\n---=== [ Final Results ] ===---\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               proc[i].id, proc[i].arrival_time, proc[i].burst_time,
               proc[i].completion_time, proc[i].turnaround_time, proc[i].waiting_time);
        total_wt += proc[i].waiting_time;
        total_tat += proc[i].turnaround_time;
    }

    printf("--------------------------------------------------\n");
    printf("Average Waiting Time:    %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n\n", total_tat / n);
}