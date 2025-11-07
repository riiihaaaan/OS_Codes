#include <stdio.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int main() {
    int n, m;
    int alloc[MAX_PROCESSES][MAX_RESOURCES];   // Allocation matrix
    int max[MAX_PROCESSES][MAX_RESOURCES];     // Maximum demand matrix
    int need[MAX_PROCESSES][MAX_RESOURCES];    // Need matrix = max - alloc
    int avail[MAX_RESOURCES];                  // Available resources
    int finish[MAX_PROCESSES];                 // Finish flag for each process
    int safeSeq[MAX_PROCESSES];                // Safe sequence (if exists)
    int i, j, k, count = 0;

    // Read number of processes
    printf("Enter number of processes (max %d): ", MAX_PROCESSES);
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_PROCESSES) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    // Read number of resource types
    printf("Enter number of resources (max %d): ", MAX_RESOURCES);
    if (scanf("%d", &m) != 1 || m <= 0 || m > MAX_RESOURCES) {
        printf("Invalid number of resources.\n");
        return 1;
    }

    // Read Allocation matrix
    printf("\nEnter Allocation Matrix (%d x %d):\n", n, m);
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            if (scanf("%d", &alloc[i][j]) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (alloc[i][j] < 0) {
                printf("Allocation entries must be non-negative.\n");
                return 1;
            }
        }
    }

    // Read Maximum matrix
    printf("\nEnter Maximum Matrix (%d x %d):\n", n, m);
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            if (scanf("%d", &max[i][j]) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (max[i][j] < 0) {
                printf("Maximum entries must be non-negative.\n");
                return 1;
            }
            // Optional check: max should be >= alloc for each entry
            if (max[i][j] < alloc[i][j]) {
                printf("Error: max < allocation for process %d, resource %d\n", i, j);
                return 1;
            }
        }
    }

    // Read Available resources
    printf("\nEnter Available Resources (%d values):\n", m);
    for (i = 0; i < m; i++) {
        if (scanf("%d", &avail[i]) != 1) {
            printf("Invalid input.\n");
            return 1;
        }
        if (avail[i] < 0) {
            printf("Available resources must be non-negative.\n");
            return 1;
        }
    }

    // Calculate Need matrix = Max - Allocation
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    // Print Need matrix for user verification
    printf("\nNeed Matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }

    // Initialize finish flags to 0 (false)
    for (i = 0; i < n; i++) {
        finish[i] = 0;
    }

    // Banker's safety algorithm to find a safe sequence
    while (count < n) {
        int found = 0; // tracks if we found a process that can run in this pass

        for (i = 0; i < n; i++) {
            if (finish[i] == 0) {
                int canRun = 1;
                // Check if all needed resources for process i are available
                for (j = 0; j < m; j++) {
                    if (need[i][j] > avail[j]) {
                        canRun = 0;
                        break;
                    }
                }

                if (canRun) {
                    // Simulate process i finishing and releasing its allocated resources
                    for (k = 0; k < m; k++) {
                        avail[k] += alloc[i][k];
                    }
                    safeSeq[count] = i; // record process index in safe sequence
                    count++;
                    finish[i] = 1;      // mark process as finished
                    found = 1;
                }
            }
        }

        // If no process could be allocated in this pass, system is unsafe
        if (!found) {
            printf("\nSystem is in UNSAFE STATE!\n");
            return 0;
        }
    }

    // If we reach here, a safe sequence was found
    printf("\nSystem is in SAFE STATE.\nSafe Sequence: ");
    for (i = 0; i < n; i++) {
        // Processes are printed as P0..P(n-1). Add +1 if you prefer P1..Pn
        printf("P%d ", safeSeq[i]);
    }
    printf("\n");

    return 0;
}