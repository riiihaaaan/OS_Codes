/*
 * clook.c
 * =======
 * Implementation of the C-LOOK disk scheduling algorithm.
 *
 * C-LOOK is a variant of C-SCAN. The head moves from its current position
 * to the highest-numbered request, servicing requests along the way.
 * Instead of going to the end of the disk, it immediately jumps back to the
 * lowest-numbered request and starts servicing again in the same direction.
 *
 * How it works:
 * 1. Sort all track requests.
 * 2. The head moves from its current position towards the highest request,
 *    servicing all requests in its path.
 * 3. After servicing the highest request, the head "jumps" to the lowest
 *    request without servicing anything in between.
 * 4. The head continues moving in the same direction (towards higher numbers),
 *    servicing the remaining requests.
 */

#include <stdio.h>
#include <stdlib.h>

// Comparison function for qsort to sort integers in ascending order.
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

/**
 * @brief Simulates the C-LOOK disk scheduling algorithm.
 * @param requests Array of track requests.
 * @param n Number of requests.
 * @param head The initial position of the disk head.
 */
void run_clook(int requests[], int n, int head) {
    int total_movement = 0;
    int current_head = head;

    // Sort the requests to make it easier to find the next track.
    qsort(requests, n, sizeof(int), compare);

    printf("Seek Sequence: %d", head);

    // --- Pass 1: Move from head towards the highest request ---
    for (int i = 0; i < n; i++) {
        // Service all requests that are >= the current head position.
        if (requests[i] >= current_head) {
            total_movement += abs(requests[i] - current_head);
            current_head = requests[i];
            printf(" -> %d", current_head);
        }
    }

    // --- Jump to the lowest request and continue ---
    // The head jumps from the last serviced (highest) request to the first (lowest) request.
    total_movement += abs(requests[0] - current_head);
    current_head = requests[0];
    printf(" -> %d", current_head);

    // --- Pass 2: Move from the lowest request up to the initial head position ---
    for (int i = 1; i < n; i++) {
        // Service the remaining requests (those that were < initial head).
        if (requests[i] < head) {
            total_movement += abs(requests[i] - current_head);
            current_head = requests[i];
            printf(" -> %d", current_head);
        } else {
            // Since the array is sorted, we can stop once we pass the initial head's zone.
            break;
        }
    }
    printf("\nTotal head movement: %d\n", total_movement);
}

int main() {
    int n, head;
    printf("Enter number of requests: ");
    scanf("%d", &n);

    int* requests = malloc(n * sizeof(int));
    printf("Enter requests:\n");
    for(int i = 0; i < n; i++) scanf("%d", &requests[i]);

    printf("Enter head: ");
    scanf("%d", &head);

    run_clook(requests, n, head);
    free(requests);
    return 0;
}
