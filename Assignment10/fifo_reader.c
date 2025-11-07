// fifo_reader.c
// Reads a message from a FIFO (named pipe).

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>     // For read()

#define FIFO_PATH "/tmp/myfifo"
#define MAX_BUF 1024

int main() {
    int fd;
    char buffer[MAX_BUF];
    int bytes_read;

    // Note: The reader doesn't need to create the FIFO, 
    // but it's good practice for one side (e.g., a server) to do it.
    // We assume the writer created it.

    printf("Reader: Opening FIFO for reading...\n");

    // 1. Open the FIFO for reading
    // The O_RDONLY call will block here until a writer opens the other end.
    fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    printf("Reader: Writer has connected.\n");

    // 2. Read from the FIFO
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("read");
    } else {
        // Add a null terminator to make it a valid string
        buffer[bytes_read] = '\0';
        printf("Reader received: %s", buffer);
    }

    // 3. Close the FIFO
    close(fd);
    
    // 4. Clean up the FIFO file from the system
    unlink(FIFO_PATH);

    return 0;
}