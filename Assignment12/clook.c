#include <stdio.h>
#include <stdlib.h>

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void run_clook(int requests[], int n, int head) {
    int total_movement = 0;
    int current_head = head;
    qsort(requests, n, sizeof(int), compare);

    printf("Seek Sequence: %d", head);

    int i;
    for (i = 0; i < n; i++) {
        if (requests[i] >= current_head) {
            total_movement += abs(requests[i] - current_head);
            current_head = requests[i];
            printf(" -> %d", current_head);
        }
    }
    total_movement += abs(requests[0] - current_head);
    current_head = requests[0];
    printf(" -> %d", current_head);

    for (i = 1; i < n; i++) {
        if (requests[i] < head) {
            total_movement += abs(requests[i] - current_head);
            current_head = requests[i];
            printf(" -> %d", current_head);
        } else {
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
