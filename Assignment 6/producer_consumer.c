// producer_consumer.c
// Demonstrates the Producer-Consumer problem using pthreads,
// a mutex, and counting semaphores.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>    // For threads and mutex
#include <semaphore.h>  // For semaphores
#include <unistd.h>     // For sleep()

#define BUFFER_SIZE 5   // Size of the shared buffer
#define MAX_ITEMS 10    // How many items to produce/consume

// --- Shared Resources ---
int buffer[BUFFER_SIZE];
int in = 0;  // Index for producer to write
int out = 0; // Index for consumer to read

pthread_mutex_t mutex;  // Mutex for buffer access
sem_t empty;            // Semaphore for empty slots
sem_t full;             // Semaphore for full slots
// ------------------------

/**
 * @brief The Producer thread function.
 */
void *producer(void *arg) {
    int item;
    for (int i = 0; i < MAX_ITEMS; i++) {
        item = rand() % 100; // Produce a random item

        // 1. Wait if the buffer is full (no empty slots)
        sem_wait(&empty);

        // 2. Lock the buffer for exclusive access
        pthread_mutex_lock(&mutex);

        // --- Critical Section ---
        buffer[in] = item;
        printf("Producer produced item %d at index %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;
        // ------------------------

        // 3. Unlock the buffer
        pthread_mutex_unlock(&mutex);

        // 4. Signal that one more slot is full
        sem_post(&full);

        sleep(1); // Simulate time taken to produce
    }
    pthread_exit(NULL);
}

/**
 * @brief The Consumer thread function.
 */
void *consumer(void *arg) {
    int item;
    for (int i = 0; i < MAX_ITEMS; i++) {
        // 1. Wait if the buffer is empty (no full slots)
        sem_wait(&full);

        // 2. Lock the buffer for exclusive access
        pthread_mutex_lock(&mutex);

        // --- Critical Section ---
        item = buffer[out];
        printf("Consumer consumed item %d from index %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        // ------------------------

        // 3. Unlock the buffer
        pthread_mutex_unlock(&mutex);

        // 4. Signal that one more slot is empty
        sem_post(&empty);

        sleep(2); // Simulate time taken to consume
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t prod_thread, cons_thread;

    // --- Initialization ---
    // 1. Initialize the mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex init failed");
        return 1;
    }

    // 2. Initialize the 'empty' semaphore
    //    sem_init(semaphore, pshared, value)
    //    pshared=0 means it's shared between threads of this process
    //    value=BUFFER_SIZE because all slots are initially empty
    if (sem_init(&empty, 0, BUFFER_SIZE) != 0) {
        perror("Empty semaphore init failed");
        return 1;
    }
    
    // 3. Initialize the 'full' semaphore
    //    value=0 because no slots are initially full
    if (sem_init(&full, 0, 0) != 0) {
        perror("Full semaphore init failed");
        return 1;
    }

    printf("Starting Producer and Consumer threads...\n");

    // --- Create Threads ---
    if (pthread_create(&prod_thread, NULL, producer, NULL) != 0) {
        perror("Producer thread creation failed");
        return 1;
    }
    if (pthread_create(&cons_thread, NULL, consumer, NULL) != 0) {
        perror("Consumer thread creation failed");
        return 1;
    }

    // --- Wait for Threads to Finish ---
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    printf("All items produced and consumed. Exiting.\n");

    // --- Cleanup ---
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}