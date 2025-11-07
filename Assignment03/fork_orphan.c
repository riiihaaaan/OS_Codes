/*
 * fork_orphan.c
 * =============
 * Demonstration of an Orphan Process
 *
 * This program illustrates the concept of an orphan process, which occurs
 * when a parent process terminates before its child process.
 *
 * Key Concepts:
 * 1. Process Creation (fork)
 *    - Creates a new child process.
 *
 * 2. Parent Termination
 *    - The parent process exits shortly after creating the child.
 *
 * 3. Orphan Process State
 *    - The child process continues to execute after its parent has died.
 *    - The child is "adopted" by the system's init process (PID 1).
 *    - The init process will reap the child when it finally exits.
 *
 * Note: Run 'ps -ef | grep <child_pid>' in another terminal after the
 * parent exits to observe the child's new Parent PID (PPID), which will be 1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        // --- Fork Failed ---
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // --- CHILD PROCESS ---
        printf("CHILD (PID %d): My parent is PID %d.\n", getpid(), getppid());
        printf("CHILD: Parent is about to exit. I am becoming an orphan.\n");
        sleep(5); // Keep the child running after the parent has exited
        printf("CHILD (PID %d): I am an orphan. My new parent is PID %d.\n", getpid(), getppid());
        printf("CHILD: I am exiting now.\n");
        exit(0);
    } else {
        // --- PARENT PROCESS ---
        printf("PARENT (PID %d): I created a child with PID %d.\n", getpid(), pid);
        printf("PARENT: I am exiting now, orphaning my child.\n");
        // Parent exits immediately, without waiting for the child.
        exit(0);
    }

    return 0; // This line is never reached
}
