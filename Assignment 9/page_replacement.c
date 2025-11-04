// page_replacement.c
// Simulates FIFO, LRU, and Optimal page replacement algorithms.

#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // For INT_MAX

// --- Function Prototypes ---
void run_fifo(int pages[], int n_pages, int n_frames);
void run_lru(int pages[], int n_pages, int n_frames);
void run_optimal(int pages[], int n_pages, int n_frames);
int find_page(int frames[], int n_frames, int page);
void print_frames(int frames[], int n_frames);

int main() {
    int n_frames, n_pages;

    printf("Enter the number of frames (minimum 3): ");
    scanf("%d", &n_frames);
    if (n_frames < 3) {
        printf("Frame size must be at least 3.\n");
        return 1;
    }

    printf("Enter the number of pages in the reference string: ");
    scanf("%d", &n_pages);
    if (n_pages <= 0) return 1;

    int *pages = (int*)malloc(n_pages * sizeof(int));
    printf("Enter the page reference string:\n");
    for (int i = 0; i < n_pages; i++) {
        scanf("%d", &pages[i]);
    }

    // Run simulations
    run_fifo(pages, n_pages, n_frames);
    run_lru(pages, n_pages, n_frames);
    run_optimal(pages, n_pages, n_frames);

    free(pages);
    return 0;
}

/**
 * @brief Helper to check if a page is in a frame.
 * @return Index if found, -1 otherwise.
 */
int find_page(int frames[], int n_frames, int page) {
    for (int i = 0; i < n_frames; i++) {
        if (frames[i] == page) return i;
    }
    return -1;
}

/**
 * @brief Helper to print the current state of frames.
 */
void print_frames(int frames[], int n_frames) {
    printf("\tFrames: ");
    for (int i = 0; i < n_frames; i++) {
        if (frames[i] == -1) printf("[ ] ");
        else printf("[%d] ", frames[i]);
    }
}

/**
 * @brief Simulates First-In, First-Out (FIFO)
 */
void run_fifo(int pages[], int n_pages, int n_frames) {
    int *frames = (int*)calloc(n_frames, sizeof(int));
    for(int i = 0; i < n_frames; i++) frames[i] = -1; // -1 indicates empty
    
    int page_faults = 0;
    int victim_index = 0; // Tracks the oldest page

    printf("\n---=== [ FIFO Simulation ] ===---\n");
    for (int i = 0; i < n_pages; i++) {
        printf("Ref: %d", pages[i]);
        if (find_page(frames, n_frames, pages[i]) == -1) { // Page Fault
            page_faults++;
            frames[victim_index] = pages[i];
            victim_index = (victim_index + 1) % n_frames;
            print_frames(frames, n_frames);
            printf("(Fault)\n");
        } else {
            print_frames(frames, n_frames);
            printf("(Hit)\n");
        }
    }
    printf("Total Page Faults (FIFO): %d\n", page_faults);
    free(frames);
}

/**
 * @brief Simulates Least Recently Used (LRU)
 */
void run_lru(int pages[], int n_pages, int n_frames) {
    int *frames = (int*)calloc(n_frames, sizeof(int));
    int *time_since_use = (int*)calloc(n_frames, sizeof(int)); // Counter
    for(int i = 0; i < n_frames; i++) frames[i] = -1;

    int page_faults = 0;

    printf("\n---=== [ LRU Simulation ] ===---\n");
    for (int i = 0; i < n_pages; i++) {
        printf("Ref: %d", pages[i]);
        int found_index = find_page(frames, n_frames, pages[i]);

        if (found_index == -1) { // Page Fault
            page_faults++;
            // Find the LRU page (one with the largest time_since_use)
            int lru_index = 0;
            for (int j = 1; j < n_frames; j++) {
                if (frames[j] == -1) { // Found an empty slot
                    lru_index = j;
                    break;
                }
                if (time_since_use[j] > time_since_use[lru_index]) {
                    lru_index = j;
                }
            }
            frames[lru_index] = pages[i];
            time_since_use[lru_index] = 0; // Reset its counter
            print_frames(frames, n_frames);
            printf("(Fault)\n");
        } else { // Page Hit
            time_since_use[found_index] = 0; // Reset its counter
            print_frames(frames, n_frames);
            printf("(Hit)\n");
        }

        // Increment time for all pages currently in frames
        for (int j = 0; j < n_frames; j++) {
            if (frames[j] != -1) time_since_use[j]++;
        }
    }
    printf("Total Page Faults (LRU): %d\n", page_faults);
    free(frames);
    free(time_since_use);
}

/**
 * @brief Simulates Optimal (OPT)
 */
void run_optimal(int pages[], int n_pages, int n_frames) {
    int *frames = (int*)calloc(n_frames, sizeof(int));
    for(int i = 0; i < n_frames; i++) frames[i] = -1;

    int page_faults = 0;

    printf("\n---=== [ Optimal Simulation ] ===---\n");
    for (int i = 0; i < n_pages; i++) {
        printf("Ref: %d", pages[i]);
        if (find_page(frames, n_frames, pages[i]) == -1) { // Page Fault
            page_faults++;
            
            // Find an empty frame first
            int empty_index = find_page(frames, n_frames, -1);
            if (empty_index != -1) {
                frames[empty_index] = pages[i];
            } else {
                // All frames full, find the optimal page to replace
                int replace_index = -1;
                int farthest_use = -1;

                for (int j = 0; j < n_frames; j++) {
                    // Look ahead in the future string
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
                    if (k == n_pages) { // This page is not used again
                        replace_index = j;
                        goto found_victim; // Easiest victim, replace it
                    }
                }
                found_victim:
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