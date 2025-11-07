/*
 * lru.c
 * =====
 * Implementation of the LRU (Least Recently Used) page replacement algorithm.
 *
 * LRU replaces the page that has not been used for the longest period of time.
 * This implementation uses a counter (`time_since_use`) for each frame to track
 * how long it has been since a page was accessed.
 *
 * How it works:
 * 1. For each page reference, check if it's already in a frame (a "hit").
 * 2. If it's a hit, reset its `time_since_use` counter to 0.
 * 3. If it's not in a frame (a "fault"):
 *    a. Find an empty frame. If one exists, place the new page there.
 *    b. If no frames are empty, find the page with the highest `time_since_use`
 *       value (the LRU page) and replace it with the new page.
 *    c. Reset the new page's `time_since_use` counter to 0.
 * 4. After each reference (hit or fault), increment the `time_since_use` counter
 *    for all pages currently in frames.
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Helper to check if a page is in frames.
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
        if (frames[i] == -1) printf("[ ] "); // -1 represents an empty frame
        else printf("[%d] ", frames[i]);
    }
}

/**
 * @brief Simulates the Least Recently Used (LRU) page replacement algorithm.
 * @param pages The sequence of page references.
 * @param n_pages The total number of page references.
 * @param n_frames The number of available frames in memory.
 */
void run_lru(int pages[], int n_pages, int n_frames) {
    int *frames = (int*)calloc(n_frames, sizeof(int));
    // `time_since_use` tracks the "age" of each page in a frame.
    // A higher value means it was used less recently.
    int *time_since_use = (int*)calloc(n_frames, sizeof(int));
    for(int i = 0; i < n_frames; i++) frames[i] = -1; // Initialize frames to empty

    int page_faults = 0;

    printf("\n---=== [ LRU Simulation ] ===---\n");
    for (int i = 0; i < n_pages; i++) {
        printf("Ref: %d", pages[i]);
        int found_index = find_page(frames, n_frames, pages[i]);

        if (found_index == -1) { // Page Fault
            page_faults++;
            // Find the victim frame: either an empty one or the LRU one.
            int lru_index = 0;
            for (int j = 1; j < n_frames; j++) {
                if (frames[j] == -1) { // Prefer an empty frame if available.
                    lru_index = j;
                    break;
                }
                // Find the page with the maximum `time_since_use` value.
                if (time_since_use[j] > time_since_use[lru_index]) { 
                    lru_index = j;
                }
            }
            frames[lru_index] = pages[i]; // Replace the victim page.
            time_since_use[lru_index] = 0; // Reset the age for the new page.
            print_frames(frames, n_frames);
            printf("(Fault)\n");
        } else { // Page Hit
            time_since_use[found_index] = 0; // Reset age because it was just used.
            print_frames(frames, n_frames);
            printf("(Hit)\n");
        }

        // After each reference, increment the age of all pages currently in frames.
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
