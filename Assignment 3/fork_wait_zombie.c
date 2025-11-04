// fork_wait_zombie.c
// Demonstrates fork(), wait(), and the zombie state.

#include <stdio.h>
#include <stdlib.h>     // For exit()
#include <unistd.h>     // For fork(), sleep(), getpid()
#include <sys/wait.h>   // For wait()

int main() {
    pid_t pid; // Variable to store process ID

    // Create a new process
    pid = fork();

    if (pid < 0) {
        // --- Fork Failed ---
        perror("fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // --- CHILD PROCESS ---
        printf("CHILD (PID %d): I am the child. I am exiting now.\n", getpid());
        exit(0); // Child exits immediately
    }
    else {
        // --- PARENT PROCESS ---
        printf("PARENT (PID %d): I am the parent. My child's PID is %d.\n", getpid(), pid);
        
        printf("PARENT: I will sleep for 10 seconds...\n");
        printf("PARENT: (During this time, the child [PID %d] is a ZOMBIE)\n", pid);
        printf("PARENT: (Open another terminal and run: ps -ef | grep %d)\n", pid);
        
        sleep(10); // Parent sleeps, NOT waiting for the child

        // Now, the parent "reaps" the child
        printf("PARENT: 10 seconds are over. I will now call wait() to reap the zombie.\n");
        wait(NULL); // Parent collects the child's exit status
        
        printf("PARENT: Child reaped. My process is now done.\n");
    }

    return 0;
}