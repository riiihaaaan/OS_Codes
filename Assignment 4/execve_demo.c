// execve_demo.c
// Demonstrates the execve() system call.

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