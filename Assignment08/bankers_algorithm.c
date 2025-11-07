/*
 * bankers_algorithm.c
 * ===================
 * Implementation of the Banker's Algorithm for deadlock avoidance.
 *
 * This program determines if a system is in a "safe state." A system is in a
 * safe state if there exists a sequence of all processes, <P1, P2, ..., Pn>,
 * such that for each Pi, the resources that Pi can still request can be
 * satisfied by the currently available resources plus the resources held by
 * all Pj, with j < i.
 *
 * Key Data Structures:
 * - Allocation: How many resources of each type are currently allocated to each process.
 * - Max: The maximum number of resources of each type that each process may request.
 * - Need: The remaining resources needed by each process (Max - Allocation).
 * - Available: The number of available resources of each type.
 */

#include <stdio.h>

// Define maximum limits for processes and resources
#define MAX_P 10
#define MAX_R 10

int main() {
    // n = number of processes, m = number of resources
    int n, m, alloc[MAX_P][MAX_R], max[MAX_P][MAX_R], need[MAX_P][MAX_R], avail[MAX_R], finish[MAX_P], safeSeq[MAX_P], i, j, k, count = 0;

    // --- 1. Input from User ---
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resources: ");
    scanf("%d", &m);

    // Input the Allocation matrix
    printf("Enter Allocation Matrix:\n");
    for (i = 0; i < n; i++) for (j = 0; j < m; j++) scanf("%d", &alloc[i][j]);

    // Input the Max matrix
    printf("Enter Maximum Matrix:\n");
    for (i = 0; i < n; i++) for (j = 0; j < m; j++) scanf("%d", &max[i][j]);

    // Input the Available resources vector
    printf("Enter Available Resources:\n");
    for (i = 0; i < m; i++) scanf("%d", &avail[i]);

    // --- 2. Calculate the Need Matrix ---
    // Need[i][j] = Max[i][j] - Allocation[i][j]
    for (i = 0; i < n; i++) for (j = 0; j < m; j++) need[i][j] = max[i][j] - alloc[i][j];

    // --- 3. Initialize the Finish Array ---
    // finish[i] = 0 means process i is not finished yet.
    for (i = 0; i < n; i++) finish[i] = 0;

    // --- 4. Safety Algorithm ---
    // This loop continues until all processes are in the safe sequence (count == n)
    // or until an unsafe state is detected.
    while (count < n) {
        int found = 0; // Flag to check if a safe process was found in the current iteration
        for (i = 0; i < n; i++) {
            // Check if the process is not yet finished
            if (finish[i] == 0) {
                int canRun = 1; // Assume the process can run
                // Check if the process's need can be met with available resources
                for (j = 0; j < m; j++) if (need[i][j] > avail[j]) canRun = 0;
                
                // If the process can run safely
                if (canRun) {
                    // Release its allocated resources and add them to the available pool
                    for (k = 0; k < m; k++) avail[k] += alloc[i][k];
                    safeSeq[count++] = i; // Add the process to the safe sequence
                    finish[i] = 1;        // Mark the process as finished
                    found = 1;            // A safe process was found
                }
            }
        }
        // If no safe process was found in a full iteration, the system is in an unsafe state.
        if (!found) {
            printf("Unsafe\n");
            return 0;
        }
    }

    // --- 5. Output the Safe Sequence ---
    printf("Safe Sequence: ");
    for (i = 0; i < n; i++) printf("P%d ", safeSeq[i]);
    printf("\n");

    return 0;
}

/*
 * --- DEMO INPUT & OUTPUT ---
 *
 * Enter number of processes: 5
 * Enter number of resources: 3
 *
 * Enter Allocation Matrix:
 * 0 1 0  <-- P0 holds 0 of R0, 1 of R1, 0 of R2
 * 2 0 0  <-- P1 holds 2 of R0, 0 of R1, 0 of R2
 * 3 0 2  <-- P2 holds 3 of R0, 0 of R1, 2 of R2
 * 2 1 1  <-- P3 holds 2 of R0, 1 of R1, 1 of R2
 * 0 0 2  <-- P4 holds 0 of R0, 0 of R1, 2 of R2

 *
 * Enter Maximum Matrix:
 * 7 5 3
 * 3 2 2
 * 9 0 2
 * 2 2 2
 * 4 3 3
 *
 * Enter Available Resources:
 * 3 3 2
 * // There are 3 instances of resource R0 available.
      There are 3 instances of resource R1 available.
      There are 2 instances of resource R2 available
 * 
 * Safe Sequence: P1 P3 P4 P0 P2
 */
