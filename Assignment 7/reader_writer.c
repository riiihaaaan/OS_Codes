/*
 * reader_writer.c
 * ==============
 * Implementation of the Classical Readers-Writers Problem with Reader Priority
 *
 * Problem Description:
 * - Multiple readers can read data simultaneously
 * - Only one writer can write at a time
 * - No reader can read while a writer is writing
 * - Writers must wait for all readers to finish
 * 
 * Solution Implementation:
 * - Uses pthreads for concurrent execution
 * - Uses two mutex locks for synchronization:
 *   1. mutex: Protects the reader count
 *   2. rw_mutex: Controls access to shared data
 * - Implements reader priority (readers get preference over writers)
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/* --- Shared Resources and Synchronization Objects --- */
int shared_data = 10;     // The shared resource that threads will access
int read_count = 0;       // Tracks number of active readers
pthread_mutex_t mutex;    // Controls access to read_count
pthread_mutex_t rw_mutex; // Controls access to shared_data
/* -------------------------------------------------- */

/**
 * @brief The Writer thread function.
 */
void *writer(void *arg) {
    int id = *(int *)arg;
    int new_value;

    printf("Writer %d: Trying to write...\n", id);
    
    // 1. Lock the resource exclusively
    pthread_mutex_lock(&rw_mutex);

    // --- Critical Section (Writing) ---
    printf("Writer %d: Is WRITING.\n", id);
    new_value = rand() % 100;
    shared_data = new_value;
    printf("Writer %d: Wrote value %d.\n", id, new_value);
    sleep(1);
    // ----------------------------------

    // 2. Unlock the resource
    pthread_mutex_unlock(&rw_mutex);
    
    printf("Writer %d: Finished writing.\n", id);
    free(arg);
    pthread_exit(NULL);
}

/**
 * @brief The Reader thread function.
 */
void *reader(void *arg) {
    int id = *(int *)arg;

    printf("Reader %d: Trying to read...\n", id);

    // --- Entry Section (Managing read_count) ---
    // 1. Lock 'mutex' to protect 'read_count'
    pthread_mutex_lock(&mutex);
    read_count++;
    
    // 2. If this is the FIRST reader, it must lock the 
    //    resource to block any waiting writers.
    if (read_count == 1) {
        printf("Reader %d: I am the first reader. Locking rw_mutex.\n", id);
        pthread_mutex_lock(&rw_mutex);
    }
    // 3. Unlock 'mutex'
    pthread_mutex_unlock(&mutex);

    // --- Critical Section (Reading) ---
    printf("Reader %d: Is READING. Shared value is %d.\n", id, shared_data);
    sleep(1);
    // ----------------------------------

    // --- Exit Section (Managing read_count) ---
    // 4. Lock 'mutex' to protect 'read_count' again
    pthread_mutex_lock(&mutex);
    read_count--;

    // 5. If this is the LAST reader, it must unlock the
    //    resource so a waiting writer can proceed.
    if (read_count == 0) {
        printf("Reader %d: I am the last reader. Unlocking rw_mutex.\n", id);
        pthread_mutex_unlock(&rw_mutex);
    }
    // 6. Unlock 'mutex'
    pthread_mutex_unlock(&mutex);
    
    printf("Reader %d: Finished reading.\n", id);
    free(arg);
    pthread_exit(NULL);
}

int main() {
    int num_readers = 5;
    int num_writers = 2;
    pthread_t readers[num_readers];
    pthread_t writers[num_writers];

    // --- Initialization ---
    if (pthread_mutex_init(&mutex, NULL) != 0 || pthread_mutex_init(&rw_mutex, NULL) != 0) {
        perror("Mutex init failed");
        return 1;
    }

    printf("Spawning %d Readers and %d Writers...\n\n", num_readers, num_writers);

    // --- Create Writer Threads ---
    for (int i = 0; i < num_writers; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        if (pthread_create(&writers[i], NULL, writer, id) != 0) {
            perror("Writer thread creation failed");
        }
    }

    // --- Create Reader Threads ---
    for (int i = 0; i < num_readers; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        if (pthread_create(&readers[i], NULL, reader, id) != 0) {
            perror("Reader thread creation failed");
        }
        sleep(1); // Stagger thread creation
    }

    // --- Wait for all threads to finish ---
    for (int i = 0; i < num_readers; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < num_writers; i++) {
        pthread_join(writers[i], NULL);
    }

    // --- Cleanup ---
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&rw_mutex);

    return 0;
}