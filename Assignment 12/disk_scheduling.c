// disk_scheduling.c
// Simulates SSTF, SCAN, and C-LOOK disk scheduling algorithms.

#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // For INT_MAX

// --- Function Prototypes ---
void run_sstf(int requests[], int n, int head);
void run_scan(int requests[], int n, int head, int disk_size);
void run_clook(int requests[], int n, int head);

// Comparison function for qsort
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main() {
    int n, head;
    int disk_size = 200; // Standard disk size is 0-199

    printf("Enter the number of disk requests: ");
    scanf("%d", &n);
    if (n <= 0) return 1;

    // Allocate memory for 3 copies of the requests
    int* req_sstf = (int*)malloc(n * sizeof(int));
    int* req_scan = (int*)malloc(n * sizeof(int));
    int* req_clook = (int*)malloc(n * sizeof(int));

    printf("Enter the disk request queue (e.g., 98 183 37 ...):\n");
    for (int i = 0; i < n; i++) {
        int req;
        scanf("%d", &req);
        req_sstf[i] = req_scan[i] = req_clook[i] = req;
    }

    printf("Enter the initial head position: ");
    scanf("%d", &head);

    // Run simulations
    run_sstf(req_sstf, n, head);
    run_scan(req_scan, n, head, disk_size);
    run_clook(req_clook, n, head);

    free(req_sstf);
    free(req_scan);
    free(req_clook);
    return 0;
}

/**
 * @brief Simulates Shortest Seek Time First (SSTF)
 */
void run_sstf(int requests[], int n, int head) {
    int total_movement = 0;
    int current_head = head;
    int serviced_count = 0;
    
    // Use an array to mark which requests have been serviced
    int* serviced = (int*)calloc(n, sizeof(int));

    printf("\n---=== [ SSTF Simulation ] ===---\n");
    printf("Seek Sequence: %d", head);

    while (serviced_count < n) {
        int min_dist = INT_MAX;
        int next_idx = -1;

        // Find the closest unserviced request
        for (int i = 0; i < n; i++) {
            if (!serviced[i]) {
                int dist = abs(requests[i] - current_head);
                if (dist < min_dist) {
                    min_dist = dist;
                    next_idx = i;
                }
            }
        }
        
        total_movement += min_dist;
        current_head = requests[next_idx];
        printf(" -> %d", current_head);
        serviced[next_idx] = 1; // Mark as serviced
        serviced_count++;
    }
    printf("\nTotal head movement (SSTF): %d\n", total_movement);
    free(serviced);
}

/**
 * @brief Simulates SCAN (Elevator) Algorithm
 */
void run_scan(int requests[], int n, int head, int disk_size) {
    int total_movement = 0;
    int current_head = head;
    
    qsort(requests, n, sizeof(int), compare);

    printf("\n---=== [ SCAN Simulation ] ===---\n");
    printf("Seek Sequence: %d", head);

    // Initial direction: "away from spindle" (towards higher numbers)
    // 1. Service all requests from head to the end of the disk
    for (int i = 0; i < n; i++) {
        if (requests[i] >= current_head) {
            total_movement += abs(requests[i] - current_head);
            current_head = requests[i];
            printf(" -> %d", current_head);
        }
    }
    
    // 2. Go to the end of the disk (track 199)
    total_movement += abs(disk_size - 1 - current_head);
    current_head = disk_size - 1;
    printf(" -> %d", current_head);

    // 3. Reverse direction and service remaining requests
    for (int i = n - 1; i >= 0; i--) {
        if (requests[i] < head) {
            total_movement += abs(requests[i] - current_head);
            current_head = requests[i];
            printf(" -> %d", current_head);
        }
    }
    printf("\nTotal head movement (SCAN): %d\n", total_movement);
}

/**
 * @brief Simulates C-LOOK Algorithm
 */
void run_clook(int requests[], int n, int head) {
    int total_movement = 0;
    int current_head = head;
    
    qsort(requests, n, sizeof(int), compare);
    
    printf("\n---=== [ C-LOOK Simulation ] ===---\n");
    printf("Seek Sequence: %d", head);

    // 1. Service all requests from head to the highest request
    int i;
    for (i = 0; i < n; i++) {
        if (requests[i] >= current_head) {
            total_movement += abs(requests[i] - current_head);
            current_head = requests[i];
            printf(" -> %d", current_head);
        }
    }
    
    // 2. "LOOK" and jump to the lowest request
    total_movement += abs(requests[0] - current_head);
    current_head = requests[0];
    printf(" -> %d", current_head);
    
    // 3. Service remaining requests up to the initial head position
    for (i = 1; i < n; i++) {
        if (requests[i] < head) {
            total_movement += abs(requests[i] - current_head);
            current_head = requests[i];
            printf(" -> %d", current_head);
        } else {
            // We've reached the point where we started, so stop
            break; 
        }
    }
    printf("\nTotal head movement (C-LOOK): %d\n", total_movement);
}