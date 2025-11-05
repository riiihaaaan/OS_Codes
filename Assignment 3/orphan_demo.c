/*
 * orphan_demo.c
 * ============
 * Demonstration of Orphan Process State in Unix Systems
 *
 * An orphan process is created when a parent process terminates
 * before its child process. This program demonstrates:
 *
 * 1. Process Hierarchy
 *    - Initially: child's parent is our program
 *    - After parent exits: child is adopted by init process (PID 1)
 *
 * 2. Process States
 *    - Parent: Creates child then exits immediately
 *    - Child: Continues running, becomes orphaned
 *    - Init: Adopts the orphaned process
 *
 * Key Observations:
 * - getppid() shows different parent PIDs before/after orphaning
 * - Init process (PID 1) automatically adopts orphaned processes
 * - Unlike zombies, orphans don't waste system resources
 */

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