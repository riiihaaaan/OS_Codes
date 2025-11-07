/*
 * exec_states.c
 * =============
 * Demonstration of EXECVE with Normal, Zombie, and Orphan States
 *
 * This program uses fork() and execve() to demonstrate various process
 * states that can occur when creating child processes that execute
 * new programs.
 *
 * Key Concepts:
 * 1. execve(): Replaces the current process image with a new one.
 *
 * 2. Normal Execution:
 *    - The parent process waits for the child to complete. The child
 *      executes a new program and exits, and the parent reaps it.
 *
 * 3. Zombie State:
 *    - A child process calls execve() and exits.
 *    - The parent does not immediately call wait(), leaving the child
 *      process entry in the process table as a "zombie".
 *
 * 4. Orphan State:
 *    - The parent process exits before the child.
 *    - The child becomes an "orphan" and is adopted by the init process
 *      (PID 1), which will wait for it upon its termination.
 *
 * Usage:
 *   gcc exec_states.c -o exec_states
 *   ./exec_states normal
 *   ./exec_states zombie
 *   ./exec_states orphan
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

/**
 * @brief Main function to demonstrate process states.
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <normal|zombie|orphan>\n", argv[0]);
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    // --- Child Process ---
    if (pid == 0) {
        printf("CHILD (PID: %d, Parent PID: %d): Process created.\n", getpid(), getppid());

        if (strcmp(argv[1], "orphan") == 0) {
            // For the orphan case, sleep first to show the parent exiting.
            sleep(2);
            printf("CHILD (PID: %d): My parent has exited. My new parent is PID: %d.\n", getpid(), getppid());
        }

        printf("CHILD (PID: %d): Now executing 'ls -l'.\n", getpid());
        printf("----------------------------------------\n");

        char *exec_args[] = {"/bin/ls", "-l", NULL};
        execve("/bin/ls", exec_args, NULL);

        // execve() only returns on error
        perror("execve failed");
        exit(EXIT_FAILURE);
    }
    // --- Parent Process ---
    else {
        printf("PARENT (PID: %d): Created child with PID: %d.\n", getpid(), pid);

        if (strcmp(argv[1], "normal") == 0) {
            printf("PARENT: Waiting for child to complete (Normal execution).\n");
            wait(NULL); // Wait for the child to finish
            printf("PARENT: Child has been reaped. Exiting.\n");

        } else if (strcmp(argv[1], "zombie") == 0) {
            printf("PARENT: Not waiting for the child. Sleeping for 10 seconds.\n");
            printf("PARENT: The child will become a zombie after it finishes 'ls -l'.\n");
            printf("PARENT: (In another terminal, run 'ps aux | grep %d' to see the zombie state.)\n", pid);
            sleep(10);
            wait(NULL); // Clean up the zombie process
            printf("PARENT: Reaped the zombie child. Exiting.\n");

        } else if (strcmp(argv[1], "orphan") == 0) {
            printf("PARENT: Exiting immediately, orphaning the child.\n");
            printf("PARENT: (The child will be adopted by init/systemd and will run to completion.)\n");
            exit(EXIT_SUCCESS); // Exit without waiting

        } else {
            fprintf(stderr, "Invalid argument '%s'. Use 'normal', 'zombie', or 'orphan'.\n", argv[1]);
            // Kill the stray child process before exiting
            kill(pid, SIGKILL);
            wait(NULL);
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}