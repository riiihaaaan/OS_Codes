// orphan_demo.c
// Demonstrates the orphan state.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // For fork(), sleep(), getpid(), getppid()

int main() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        // --- Fork Failed ---
        perror("fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // --- CHILD PROCESS ---
        printf("CHILD (PID %d): I am the child. My parent is %d.\n", getpid(), getppid());
        
        printf("CHILD: My parent will exit, and I will become an orphan.\n");
        printf("CHILD: I will sleep for 10 seconds...\n");
        sleep(10); // Child sleeps, becoming an orphan
        
        printf("CHILD: 10 seconds are over. My new parent (adopted by init) is %d.\n", getppid());
        printf("CHILD: I am exiting now.\n");
        exit(0);
    }
    else {
        // --- PARENT PROCESS ---
        printf("PARENT (PID %d): I am the parent. I am exiting now.\n", getpid());
        exit(0); // Parent exits immediately
    }
}