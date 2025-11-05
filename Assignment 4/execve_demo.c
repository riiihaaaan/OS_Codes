/*
 * execve_demo.c
 * ============
 * Demonstration of Process Image Replacement using execve()
 *
 * The execve() system call loads a new program into the current
 * process's memory space, completely replacing the current program.
 *
 * Key Concepts:
 * 1. Process Transformation
 *    - Same process ID (PID)
 *    - New program code and data
 *    - Original program is completely gone
 *
 * 2. execve() Parameters
 *    - pathname: Full path to the new program
 *    - argv: Array of argument strings
 *    - envp: Array of environment variables
 *
 * 3. Process State
 *    - On success: Current program vanishes
 *    - On failure: Returns -1, original program continues
 *
 * Example: This program replaces itself with 'ls -l'
 */

#include <stdio.h>
#include <unistd.h>     // For execve(), getpid()
#include <stdlib.h>     // For exit()

int main() {
    printf("--- Start of execve_demo ---\n");
    printf("My PID is: %d\n", getpid());
    printf("I am about to replace myself with 'ls -l'\n");
    printf("This will be the last line you see from *this* program.\n");
    printf("----------------------------------\n\n");

    // 1. Arguments for the new program
    // The first argument (argv[0]) must be the program name itself.
    char *args[] = {"/bin/ls", "-l", NULL};
    
    // 2. Environment (can be NULL for this demo)
    char *envp[] = {NULL};

    // 3. Make the system call
    // The kernel will load "/bin/ls" into this process's memory.
    execve("/bin/ls", args, envp);

    // --- THIS CODE ONLY RUNS IF EXECVE FAILS ---
    // If execve() succeeds, our program is gone.
    // If it fails, the kernel returns -1.
    perror("execve failed");
    exit(1);
}

// To compile and run:
// gcc execve_demo.c -o exec_demo
// ./exec_demo