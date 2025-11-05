/*
 * fork_wait_zombie.c
 * =================
 * Demonstration of Process Creation, Termination and Zombie State
 *
 * This program illustrates three important Unix process concepts:
 * 1. Process Creation (fork)
 *    - Creates a new child process that is a copy of the parent
 *    - Parent and child execute concurrently
 *
 * 2. Process Termination (exit)
 *    - Child process terminates early while parent continues
 *    - If parent hasn't called wait(), child becomes zombie
 *
 * 3. Zombie Process State
 *    - Process that has terminated but whose exit status 
 *      hasn't been collected by its parent
 *    - Still consumes system resources until "reaped"
 *
 * Note: Run 'ps -ef | grep <PID>' in another terminal during
 * the sleep period to observe the zombie process state
 */

#include <stdio.h>
#include <stdlib.h>     // For exit() function
#include <unistd.h>     // For fork(), sleep(), getpid()
#include <sys/wait.h>   // For wait() function

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