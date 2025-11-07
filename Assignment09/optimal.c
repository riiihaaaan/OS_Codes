#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Helper to check if a page is in frames
int find_page(int frames[], int n_frames, int page) {
    for (int i = 0; i < n_frames; i++) {
        if (frames[i] == page) return i;
    }
    return -1;
}

// Print current frames
void print_frames(int frames[], int n_frames) {
    printf("\tFrames: ");
    for (int i = 0; i < n_frames; i++) {
        if (frames[i] == -1) printf("[ ] ");
        else printf("[%d] ", frames[i]);
    }
}

// Simulates Optimal page replacement
void run_optimal(int pages[], int n_pages, int n_frames) {
    int *frames = (int*)calloc(n_frames, sizeof(int));
    for(int i = 0; i < n_frames; i++) frames[i] = -1;

    int page_faults = 0;

    printf("\n---=== [ Optimal Simulation ] ===---\n");
    for (int i = 0; i < n_pages; i++) {
        printf("Ref: %d", pages[i]);
        if (find_page(frames, n_frames, pages[i]) == -1) { // Page Fault
            page_faults++;

            // Find empty frame first
            int empty_index = find_page(frames, n_frames, -1);
            if (empty_index != -1) {
                frames[empty_index] = pages[i];
            } else {
                // Find optimal page to replace
                int replace_index = -1;
                int farthest_use = -1;

                for (int j = 0; j < n_frames; j++) {
                    int k;
                    for (k = i + 1; k < n_pages; k++) {
                        if (frames[j] == pages[k]) {
                            if (k > farthest_use) {
                                farthest_use = k;
                                replace_index = j;
                            }
                            break;
                        }
                    }
                    if (k == n_pages) { // Not used again
                        replace_index = j;
                        break; // Found victim
                    }
                }
                frames[replace_index] = pages[i];
            }
            print_frames(frames, n_frames);
            printf("(Fault)\n");
        } else {
            print_frames(frames, n_frames);
            printf("(Hit)\n");
        }
    }
    printf("Total Page Faults (Optimal): %d\n", page_faults);
    free(frames);
}

int main() {
    int n_frames, n_pages;
    printf("Enter number of frames: ");
    scanf("%d", &n_frames);
    printf("Enter number of pages: ");
    scanf("%d", &n_pages);
    int *pages = malloc(n_pages * sizeof(int));
    printf("Enter page reference string:\n");
    for (int i = 0; i < n_pages; i++) scanf("%d", &pages[i]);
    run_optimal(pages, n_pages, n_frames);
    free(pages);
    return 0;
}
