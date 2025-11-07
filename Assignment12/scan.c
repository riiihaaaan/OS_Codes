/*
 * scan.c
 * ======
 * Implementation of the SCAN (Elevator) disk scheduling algorithm.
 *
 * The SCAN algorithm moves the disk arm in one direction, servicing all
 * requests in its path, until it reaches the end of the disk. Then, it
 * reverses direction and services requests in the other direction.
 *
 * How it works:
 * 1. Sort all track requests.
 * 2. The head moves from its current position towards one end of the disk
 *    (e.g., towards the highest track number), servicing requests along the way.
 * 3. When it reaches the end of the disk, it reverses direction.
 * 4. It then moves towards the other end, servicing the remaining requests.
 */

#include <stdio.h>
#include <stdlib.h>

// Comparison function for qsort to sort integers in ascending order.
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

/**
 * @brief Simulates the SCAN disk scheduling algorithm.
 * @param requests Array of track requests.
 * @param n Number of requests.
 * @param head The initial position of the disk head.
 * @param disk_size The total number of tracks on the disk.
 */
void run_scan(int requests[], int n, int head, int disk_size) {
    int total_movement = 0;
    int current_head = head;

    // Sort the requests to make it easier to find the next track.
    qsort(requests, n, sizeof(int), compare);

    printf("Seek Sequence: %d", head);

    // --- Pass 1: Move from head towards the end of the disk (high numbers) ---
    for (int i = 0; i < n; i++) {
        // Service all requests that are >= the current head position.
        if (requests[i] >= current_head) {
            total_movement += abs(requests[i] - current_head);
            current_head = requests[i];
            printf(" -> %d", current_head);
        }
    }

    // The head moves to the very end of the disk.
    total_movement += abs(disk_size - 1 - current_head);
    current_head = disk_size - 1;
    printf(" -> %d", current_head);

    // --- Pass 2: Move from the end back towards the start (low numbers) ---
    for (int i = n - 1; i >= 0; i--) {
        // Service the remaining requests (those that were < initial head).
        if (requests[i] < head) {
            total_movement += abs(requests[i] - current_head);
            current_head = requests[i];
            printf(" -> %d", current_head);
        }
    }

    printf("\nTotal head movement: %d\n", total_movement);
}

int main() {
    int n, head;
    int disk_size = 200; // Assuming a disk size of 200 tracks.
    printf("Enter number of requests: ");
    scanf("%d", &n);

    int* requests = malloc(n * sizeof(int));
    printf("Enter requests:\n");
    for(int i = 0; i < n; i++) scanf("%d", &requests[i]);

    printf("Enter head: ");
    scanf("%d", &head);
    run_scan(requests, n, head, disk_size);
    free(requests);
    return 0;
}
