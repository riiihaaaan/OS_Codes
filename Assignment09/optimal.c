/*
 * optimal.c
 * =========
 * Implementation of the Optimal (OPT) page replacement algorithm.
 *
 * The Optimal algorithm has the lowest page fault rate of all algorithms. It
 * works by replacing the page that will not be used for the longest period
 * of time in the future. This requires "clairvoyance" (knowledge of the future
 * reference string), making it impossible to implement in a real OS, but it
 * serves as the best-case benchmark for evaluating other algorithms.
 *
 * How it works:
 * 1. For each page reference, check if it's already in a frame (a "hit").
 * 2. If it's a hit, do nothing.
 * 3. If it's not in a frame (a "fault"):
 *    a. Find an empty frame. If one exists, place the new page there.
 *    b. If no frames are empty, look ahead in the reference string to find
 *       which page currently in a frame will be used furthest in the future
 *       (or not at all). Replace that page.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Included for INT_MAX, though not used in this version.

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
 * @brief Simulates the Optimal (OPT) page replacement algorithm.
 * @param pages The sequence of page references.
 * @param n_pages The total number of page references.
 * @param n_frames The number of available frames in memory.
 */
void run_optimal(int pages[], int n_pages, int n_frames) {
    int *frames = (int*)calloc(n_frames, sizeof(int));
    for(int i = 0; i < n_frames; i++) frames[i] = -1; // Initialize frames to empty

    int page_faults = 0;

    printf("\n---=== [ Optimal Simulation ] ===---\n");
    for (int i = 0; i < n_pages; i++) {
        printf("Ref: %d", pages[i]);
        if (find_page(frames, n_frames, pages[i]) == -1) { // --- Page Fault ---
            page_faults++;

            // First, try to find an empty frame.
            int empty_index = find_page(frames, n_frames, -1);
            if (empty_index != -1) {
                frames[empty_index] = pages[i];
            } else {
                // If no empty frames, find the optimal page to replace.
                int replace_index = -1;
                int farthest_use = -1;

                for (int j = 0; j < n_frames; j++) {
                    // Look ahead in the page reference string.
                    int k;
                    for (k = i + 1; k < n_pages; k++) {
                        if (frames[j] == pages[k]) {
                            // Found a future use. If it's farther than what we've seen, update.
                            if (k > farthest_use) {
                                farthest_use = k;
                                replace_index = j;
                            }
                            break;
                        }
                    }
                    // If the loop finished without a break, this page is never used again.
                    // This is the best page to replace.
                    if (k == n_pages) {
                        replace_index = j;
                        break; // Found the perfect victim, no need to check other frames.
                    }
                }
                frames[replace_index] = pages[i];
            }
            print_frames(frames, n_frames);
            printf("(Fault)\n");
        } else { // --- Page Hit ---
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
