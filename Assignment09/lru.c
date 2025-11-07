#include <stdio.h>
#include <stdlib.h>

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

// Simulates Least Recently Used (LRU)
void run_lru(int pages[], int n_pages, int n_frames) {
    int *frames = (int*)calloc(n_frames, sizeof(int));
    int *time_since_use = (int*)calloc(n_frames, sizeof(int));
    for(int i = 0; i < n_frames; i++) frames[i] = -1;

    int page_faults = 0;

    printf("\n---=== [ LRU Simulation ] ===---\n");
    for (int i = 0; i < n_pages; i++) {
        printf("Ref: %d", pages[i]);
        int found_index = find_page(frames, n_frames, pages[i]);

        if (found_index == -1) { // Page Fault
            page_faults++;
            // Find LRU page
            int lru_index = 0;
            for (int j = 1; j < n_frames; j++) {
                if (frames[j] == -1) {
                    lru_index = j;
                    break;
                }
                if (time_since_use[j] > time_since_use[lru_index]) {
                    lru_index = j;
                }
            }
            frames[lru_index] = pages[i];
            time_since_use[lru_index] = 0;
            print_frames(frames, n_frames);
            printf("(Fault)\n");
        } else { // Hit
            time_since_use[found_index] = 0;
            print_frames(frames, n_frames);
            printf("(Hit)\n");
        }

        // Increment time for all pages
        for (int j = 0; j < n_frames; j++) {
            if (frames[j] != -1) time_since_use[j]++;
        }
    }
    printf("Total Page Faults (LRU): %d\n", page_faults);
    free(frames);
    free(time_since_use);
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
    run_lru(pages, n_pages, n_frames);
    free(pages);
    return 0;
}
