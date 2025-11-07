/*
 * round_robin.c
 * =============
 * Implementation of CPU Scheduling Algorithm: Round Robin
 *
 * This program simulates the Round Robin (RR) scheduling algorithm.
 *
 * Key Concepts:
 * - Time Quantum: Each process gets a fixed amount of CPU time (quantum).
 * - Preemptive: If a process's burst time is longer than the quantum,
 *   it is preempted and moved to the back of the ready queue.
 * - Circular Queue: Processes are executed in a circular order.
 *
 * Performance Metrics Calculated:
 * - Completion Time (CT): The time at which a process finishes execution.
 * - Turnaround Time (TAT): CT - Arrival Time.
 * - Waiting Time (WT): TAT - Burst Time.
 */

#include <stdio.h>
#include <stdlib.h>

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
 * @brief Main function to run the Round Robin simulation.
 */
int main() {
    int n, quantum;

    printf("--- Round Robin Scheduling ---\n");
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

    printf("\nEnter the time quantum for Round Robin: ");
    scanf("%d", &quantum);
    if (quantum <= 0) {
        printf("Error: Time quantum must be positive.\n");
        free(processes);
        return 1;
    }

    int current_time = 0;
    int completed = 0;

    while (completed != n) {
        int all_idle = 1; // Flag to check if CPU was idle in a full cycle
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                all_idle = 0; // At least one process is ready

                // Determine execution time: either the quantum or what's left
                int exec_time = (processes[i].remaining_time > quantum) ? quantum : processes[i].remaining_time;

                // Execute the process
                current_time += exec_time;
                processes[i].remaining_time -= exec_time;

                // If process finished, calculate metrics
                if (processes[i].remaining_time == 0) {
                    processes[i].completion_time = current_time;
                    processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
                    processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
                    completed++;
                }
            }
        }

        // If no process was ready in that full cycle, advance time
        if (all_idle) {
            current_time++;
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