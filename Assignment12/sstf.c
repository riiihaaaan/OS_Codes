/*
 * sstf.c
 * ======
 * Implementation of the SSTF (Shortest Seek Time First) disk scheduling algorithm.
 *
 * SSTF selects the request with the minimum seek time from the current head
 * position. It's a greedy algorithm that aims to minimize seek time but can
 * lead to starvation for requests far from the head.
 *
 * How it works:
 * 1. From the current head position, find the track in the request queue
 *    that is closest.
 * 2. Move the head to that track, servicing the request.
 * 3. Repeat until all requests are serviced.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // For INT_MAX

/**
 * @brief Simulates the SSTF disk scheduling algorithm.
 * @param requests Array of track requests.
 * @param n Number of requests.
 * @param head The initial position of the disk head.
 */
void run_sstf(int requests[], int n, int head) {
    int total_movement = 0;
    int current_head = head;
    int serviced_count = 0;
    // 'serviced' array acts as a boolean flag to track completed requests.
    int* serviced = (int*)calloc(n, sizeof(int)); // Initializes to all 0s

    printf("Seek Sequence: %d", head);

    // Loop until all requests are serviced.
    while (serviced_count < n) {
        int min_dist = INT_MAX;
        int next_idx = -1;

        // Find the unserviced request closest to the current head position.
        for (int i = 0; i < n; i++) {
            if (!serviced[i]) {
                int dist = abs(requests[i] - current_head);
                if (dist < min_dist) {
                    min_dist = dist;
                    next_idx = i; // Store the index of the closest request
                }
            }
        }

        // Service the closest request found.
        total_movement += min_dist;
        current_head = requests[next_idx];
        printf(" -> %d", current_head);
        serviced[next_idx] = 1; // Mark this request as completed.
        serviced_count++;
    }
    printf("\nTotal head movement: %d\n", total_movement);
    free(serviced);
}

int main() {
    int n, head;
    printf("Enter number of requests: ");
    scanf("%d", &n);

    // Dynamically allocate memory for the requests.
    int* requests = malloc(n * sizeof(int));
    printf("Enter requests:\n");
    for(int i = 0; i < n; i++) scanf("%d", &requests[i]);

    printf("Enter head: ");
    scanf("%d", &head);

    run_sstf(requests, n, head);
    free(requests);
    return 0;
}
