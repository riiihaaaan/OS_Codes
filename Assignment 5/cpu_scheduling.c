// cpu_scheduling.c
// Simulates Preemptive SJF (SRTF) and Round Robin scheduling.

#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // For INT_MAX

// Structure to hold process information
typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    
    // Helper fields for calculation
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

// Function Prototypes
void run_srtf(Process proc[], int n);
void run_round_robin(Process proc[], int n, int quantum);
void print_results(Process proc[], int n, const char* algorithm_name);

int main() {
    int n, quantum;

    printf("Enter the number of processes: ");
    scanf("%d", &n);
    if (n <= 0) {
        printf("Must have at least one process.\n");
        return 1;
    }

    // Allocate two separate arrays for the processes
    Process* processes_srtf = (Process*)malloc(n * sizeof(Process));
    Process* processes_rr = (Process*)malloc(n * sizeof(Process));

    printf("\nEnter process details (Arrival Time Burst Time):\n");
    for (int i = 0; i < n; i++) {
        printf("Process P%d: ", i);
        processes_srtf[i].id = i;
        scanf("%d %d", &processes_srtf[i].arrival_time, &processes_srtf[i].burst_time);
        processes_srtf[i].remaining_time = processes_srtf[i].burst_time;
        
        // Copy the initial data to the second array for Round Robin
        processes_rr[i] = processes_srtf[i];
    }

    printf("\nEnter the time quantum for Round Robin: ");
    scanf("%d", &quantum);
    if (quantum <= 0) {
        printf("Time quantum must be positive.\n");
        return 1;
    }

    // --- Run Simulations ---
    run_srtf(processes_srtf, n);
    run_round_robin(processes_rr, n, quantum);

    // Free the allocated memory
    free(processes_srtf);
    free(processes_rr);
    return 0;
}

/**
 * @brief Simulates Preemptive Shortest Job First (SRTF)
 */
void run_srtf(Process proc[], int n) {
    int current_time = 0;
    int completed = 0;

    while (completed != n) {
        int shortest_idx = -1;
        int min_remaining = INT_MAX;

        // Find the process with the shortest remaining time among all
        // processes that have arrived and are not yet complete.
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= current_time && proc[i].remaining_time > 0) {
                if (proc[i].remaining_time < min_remaining) {
                    min_remaining = proc[i].remaining_time;
                    shortest_idx = i;
                }
            }
        }

        if (shortest_idx == -1) {
            // No process is ready, CPU is idle.
            current_time++;
        } else {
            // Execute the shortest process for one time unit
            proc[shortest_idx].remaining_time--;
            current_time++;

            // Check if the process has completed
            if (proc[shortest_idx].remaining_time == 0) {
                proc[shortest_idx].completion_time = current_time;
                proc[shortest_idx].turnaround_time = proc[shortest_idx].completion_time - proc[shortest_idx].arrival_time;
                proc[shortest_idx].waiting_time = proc[shortest_idx].turnaround_time - proc[shortest_idx].burst_time;
                completed++;
            }
        }
    }
    
    print_results(proc, n, "Shortest Job First (Preemptive - SRTF)");
}

/**
 * @brief Simulates Round Robin (RR)
 */
void run_round_robin(Process proc[], int n, int quantum) {
    int current_time = 0;
    int completed = 0;

    // We can just loop through the array, simulating a circular queue
    while (completed != n) {
        int all_idle = 1; // Flag to check if CPU was idle
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= current_time && proc[i].remaining_time > 0) {
                all_idle = 0; // At least one process is ready
                int exec_time = 0;

                // Determine execution time: quantum or remaining time
                if (proc[i].remaining_time > quantum) {
                    exec_time = quantum;
                } else {
                    exec_time = proc[i].remaining_time;
                }

                // Execute the process
                current_time += exec_time;
                proc[i].remaining_time -= exec_time;

                // Check if the process has completed
                if (proc[i].remaining_time == 0) {
                    proc[i].completion_time = current_time;
                    proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
                    proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
                    completed++;
                }
            }
        }
        
        // If no process was ready in that full cycle, increment time
        if (all_idle) {
            current_time++;
        }
    }

    print_results(proc, n, "Round Robin");
}

/**
 * @brief Prints a formatted table of results
 */
void print_results(Process proc[], int n, const char* algorithm_name) {
    float total_wt = 0, total_tat = 0;

    printf("\n---=== [ %s ] ===---\n", algorithm_name);
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