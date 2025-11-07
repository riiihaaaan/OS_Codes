#include <stdio.h>

#define MAX_P 10
#define MAX_R 10

int main() {
    int n, m, alloc[MAX_P][MAX_R], max[MAX_P][MAX_R], need[MAX_P][MAX_R], avail[MAX_R], finish[MAX_P], safeSeq[MAX_P], i, j, k, count = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resources: ");
    scanf("%d", &m);

    printf("Enter Allocation Matrix:\n");
    for (i = 0; i < n; i++) for (j = 0; j < m; j++) scanf("%d", &alloc[i][j]);

    printf("Enter Maximum Matrix:\n");
    for (i = 0; i < n; i++) for (j = 0; j < m; j++) scanf("%d", &max[i][j]);

    printf("Enter Available Resources:\n");
    for (i = 0; i < m; i++) scanf("%d", &avail[i]);

    for (i = 0; i < n; i++) for (j = 0; j < m; j++) need[i][j] = max[i][j] - alloc[i][j];

    for (i = 0; i < n; i++) finish[i] = 0;

    while (count < n) {
        int found = 0;
        for (i = 0; i < n; i++) {
            if (!finish[i]) {
                int canRun = 1;
                for (j = 0; j < m; j++) if (need[i][j] > avail[j]) canRun = 0;
                if (canRun) {
                    for (k = 0; k < m; k++) avail[k] += alloc[i][k];
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) {
            printf("Unsafe\n");
            return 0;
        }
    }

    printf("Safe Sequence: ");
    for (i = 0; i < n; i++) printf("P%d ", safeSeq[i]);
    printf("\n");

    return 0;
}
