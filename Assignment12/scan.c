#include <stdio.h>
#include <stdlib.h>

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void run_scan(int requests[], int n, int head, int disk_size) {
    int total_movement = 0;
    int current_head = head;
    qsort(requests, n, sizeof(int), compare);

    printf("Seek Sequence: %d", head);

    for (int i = 0; i < n; i++) {
        if (requests[i] >= current_head) {
            total_movement += abs(requests[i] - current_head);
            current_head = requests[i];
            printf(" -> %d", current_head);
        }
    }
    total_movement += abs(disk_size - 1 - current_head);
    current_head = disk_size - 1;
    printf(" -> %d", current_head);

    for (int i = n - 1; i >= 0; i--) {
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
    int disk_size = 200;
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
