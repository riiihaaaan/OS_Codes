// fifo_writer.c
// Writes a message to a FIFO (named pipe).

#include <stdio.h>
#include <string.h>
#include <fcntl.h>      // For open()
#include <sys/stat.h>   // For mkfifo()
#include <sys/types.h>
#include <unistd.h>     // For write()

#define FIFO_PATH "/tmp/myfifo" // Path for the named pipe

int main() {
    int fd;
    char message[100];

    // 1. Create the FIFO (named pipe)
    // mkfifo returns 0 on success, -1 on error (e.g., if file already exists)
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        perror("mkfifo");
        // Don't exit if it already exists, we can still use it
    }

    printf("Writer: Waiting for reader to open the FIFO...\n");

    // 2. Open the FIFO for writing
    // The O_WRONLY call will block here until a reader opens the other end.
    fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    printf("Writer: Reader has connected. Enter a message: ");
    
    // 3. Read a message from the user
    fgets(message, sizeof(message), stdin);

    // 4. Write the message to the FIFO
    if (write(fd, message, strlen(message)) == -1) {
        perror("write");
    }

    // 5. Close the FIFO
    close(fd);
    printf("Writer: Message sent. Exiting.\n");

    return 0;
}