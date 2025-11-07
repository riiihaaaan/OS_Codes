/*
 * bankers_algorithm.c
 * ==================
 * Implementation of Dijkstra's Banker's Algorithm for Deadlock Avoidance
 * 
 * This program simulates a banking system that ensures a safe state
 * for resource allocation, preventing deadlocks in an operating system.
 * It implements:
 * 1. Safety Algorithm: Determines if the system is in a safe state
 * 2. Resource-Request Algorithm: Handles new resource requests safely
 *
 * Key Concepts:
 * - Available: Resources available for allocation
 * - Max: Maximum resources a process can request
 * - Allocation: Resources currently allocated to processes
 * - Need: Remaining resources a process may need (Max - Allocation)
 */

#include <stdio.h>
#include <stdbool.h>

/* Constants for maximum array sizes */
#define MAX_PROCESSES 10  // Maximum number of processes
#define MAX_RESOURCES 10  // Maximum number of resource types

// --- Function Prototypes ---
void get_input(int *n, int *m, int allocation[][MAX_RESOURCES], int max[][MAX_RESOURCES], int available[]);
void calculate_need(int n, int m, int allocation[][MAX_RESOURCES], int max[][MAX_RESOURCES], int need[][MAX_RESOURCES]);
bool run_safety_algorithm(int n, int m, int allocation[][MAX_RESOURCES], int need[][MAX_RESOURCES], int available[], int safe_sequence[]);
void handle_resource_request(int n, int m, int allocation[][MAX_RESOURCES], int need[][MAX_RESOURCES], int available[]);

int main() {
    int n, m; // n = processes, m = resources
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int max[MAX_PROCESSES][MAX_RESOURCES];
    int available[MAX_RESOURCES];
    int need[MAX_PROCESSES][MAX_RESOURCES];
    int safe_sequence[MAX_PROCESSES];

    // --- 1. Get all inputs ---
    get_input(&n, &m, allocation, max, available);

    // --- 2. Calculate the Need matrix ---
    calculate_need(n, m, allocation, max, need);

    // --- 3. Check if the initial state is safe ---
    printf("\n--- Checking Initial System State ---\n");
    if (run_safety_algorithm(n, m, allocation, need, available, safe_sequence)) {
        printf("System is in a SAFE state.\nSafe Sequence: ");
        for (int i = 0; i < n; i++) {
            printf("P%d ", safe_sequence[i]);
        }
        printf("\n");
    } else {
        printf("System is in an UNSAFE state.\n");
    }

    // --- 4. Handle a new resource request ---
    handle_resource_request(n, m, allocation, need, available);

    return 0;
}

/**
 * @brief Gets all required inputs from the user.
 */
void get_input(int *n, int *m, int allocation[][MAX_RESOURCES], int max[][MAX_RESOURCES], int available[]) {
    printf("Enter number of processes: ");
    scanf("%d", n);
    printf("Enter number of resource types: ");
    scanf("%d", m);

    printf("\nEnter Allocation Matrix (Process x Resource):\n");
    for (int i = 0; i < *n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < *m; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    printf("\nEnter Max Matrix (Process x Resource):\n");
    for (int i = 0; i < *n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < *m; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    printf("\nEnter Available Resources Vector:\n");
    for (int j = 0; j < *m; j++) {
        scanf("%d", &available[j]);
    }
}

/**
 * @brief Calculates the Need matrix (Need = Max - Allocation).
 */
void calculate_need(int n, int m, int allocation[][MAX_RESOURCES], int max[][MAX_RESOURCES], int need[][MAX_RESOURCES]) {
    printf("\n--- Need Matrix (Max - Allocation) ---\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
}

/**
 * @brief Runs the Safety Algorithm to check if the state is safe.
 * @return true if safe, false otherwise.
 */
bool run_safety_algorithm(int n, int m, int allocation[][MAX_RESOURCES], int need[][MAX_RESOURCES], int available[], int safe_sequence[]) {
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};
    int seq_count = 0;

    // 1. Initialize Work = Available
    for (int j = 0; j < m; j++) {
        work[j] = available[j];
    }

    // 2. Find a process Pi such that Finish[i]==false and Need[i] <= Work
    int pass = 0;
    while (seq_count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (finish[i] == false) {
                // Check if Need[i] <= Work
                bool can_allocate = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }

                if (can_allocate) {
                    // 3. Work = Work + Allocation[i], Finish[i] = true
                    for (int j = 0; j < m; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    safe_sequence[seq_count++] = i;
                    found = true;
                }
            }
        }
        
        // If no such process was found in a full pass, the system is unsafe
        if (found == false) {
            return false;
        }
    }

    // 4. If all processes are finished, the system is safe
    return true;
}

/**
 * @brief Runs the Resource-Request Algorithm.
 */
void handle_resource_request(int n, int m, int allocation[][MAX_RESOURCES], int need[][MAX_RESOURCES], int available[]) {
    int pid;
    int request[MAX_RESOURCES];
    int safe_seq[MAX_PROCESSES];

    printf("\n--- Resource Request Algorithm ---\n");
    printf("Enter process ID making the request (e.g., 0, 1, ...): ");
    scanf("%d", &pid);
    if (pid < 0 || pid >= n) {
        printf("Invalid process ID.\n");
        return;
    }

    printf("Enter request vector for P%d: ", pid);
    for (int j = 0; j < m; j++) {
        scanf("%d", &request[j]);
    }

    // 1. Check if Request <= Need
    for (int j = 0; j < m; j++) {
        if (request[j] > need[pid][j]) {
            printf("Error: Request exceeds maximum need. Request denied.\n");
            return;
        }
    }

    // 2. Check if Request <= Available
    for (int j = 0; j < m; j++) {
        if (request[j] > available[j]) {
            printf("Resources not available. Process P%d must wait.\n", pid);
            return;
        }
    }

    // 3. Pretend to grant the request
    for (int j = 0; j < m; j++) {
        available[j] -= request[j];
        allocation[pid][j] += request[j];
        need[pid][j] -= request[j];
    }

    // 4. Run the safety algorithm on the new state
    if (run_safety_algorithm(n, m, allocation, need, available, safe_seq)) {
        printf("Request can be granted. System remains in a SAFE state.\n");
    } else {
        printf("Request cannot be granted. Granting it would lead to an UNSAFE state.\n");
        // Roll back the changes
        for (int j = 0; j < m; j++) {
            available[j] += request[j];
            allocation[pid][j] -= request[j];
            need[pid][j] += request[j];
        }
    }
}