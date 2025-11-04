// shm_client.c
// Attaches and reads from an existing shared memory segment.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SHM_SIZE 1024

int main() {
    key_t key;
    int shmid;
    char *shm_ptr;

    // 1. Generate the *same* key as the server
    key = ftok("shmfile", 65);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // 2. Locate the shared memory segment
    // Note: We do not use IPC_CREAT here.
    shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    printf("Client: Located shared memory segment (ID: %d)\n", shmid);

    // 3. Attach the segment to the client's address space
    shm_ptr = (char*) shmat(shmid, NULL, 0);
    if (shm_ptr == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // 4. Read data from the shared memory
    printf("Client: Reading from shared memory...\n");
    printf("Client: Message received: %s", shm_ptr);
    
    // 5. Write '*' to the segment to signal the server
    *shm_ptr = '*';
    printf("Client: Wrote '*' to signal server.\n");

    // 6. Detach from the segment
    if (shmdt(shm_ptr) == -1) {
        perror("shmdt");
        exit(1);
    }

    printf("Client: Detached. Exiting.\n");
    return 0;
}