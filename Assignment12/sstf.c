#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void run_sstf(int requests[], int n, int head) {
    int total_movement = 0;
    int current_head = head;
    int serviced_count = 0;
    int* serviced = (int*)calloc(n, sizeof(int));

    printf("Seek Sequence: %d", head);

    while (serviced_count < n) {
        int min_dist = INT_MAX;
        int next_idx = -1;
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
        serviced[next_idx] = 1;
        serviced_count++;
    }
    printf("\nTotal head movement: %d\n", total_movement);
    free(serviced);
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
    run_sstf(requests, n, head);
    free(requests);
    return 0;
}
