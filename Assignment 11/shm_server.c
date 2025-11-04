// shm_server.c
// Creates, attaches, and writes to a shared memory segment.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>    // For IPC (Inter-Process Communication)
#include <sys/shm.h>    // For shared memory functions
#include <sys/types.h>
#include <unistd.h>     // For sleep()

#define SHM_SIZE 1024  // 1 KB

int main() {
    key_t key;
    int shmid;
    char *shm_ptr;
    char buffer[100];

    // 1. Generate a unique key
    // ftok (file-to-key) creates a unique key based on a file path and an ID.
    // We'll use "shmfile" as a dummy file name.
    key = ftok("shmfile", 65);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // 2. Create the shared memory segment
    // shmget(key, size, flags)
    // IPC_CREAT: Create the segment if it doesn't exist.
    // 0666: Read/write permissions for all users.
    shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    printf("Server: Shared memory segment created (ID: %d)\n", shmid);

    // 3. Attach the segment to the server's address space
    // shmat(id, address, flags)
    // NULL address lets the kernel choose a suitable address.
    shm_ptr = (char*) shmat(shmid, NULL, 0);
    if (shm_ptr == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // 4. Write data to the shared memory
    printf("Server: Enter a message to write: ");
    fgets(buffer, sizeof(buffer), stdin);
    strcpy(shm_ptr, buffer);
    printf("Server: Message written to shared memory.\n");
    
    // We put the server to sleep. The client will read the data,
    // write a '*' to the memory, and exit. The server will then wake up.
    while (*shm_ptr != '*') {
        printf("Server: Waiting for client to read...\n");
        sleep(1);
    }

    // 5. Detach from the segment
    if (shmdt(shm_ptr) == -1) {
        perror("shmdt");
        exit(1);
    }
    
    // 6. Destroy the shared memory segment
    // IPC_RMID marks the segment for destruction.
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }
    
    printf("Server: Shared memory detached and destroyed. Exiting.\n");
    return 0;
}