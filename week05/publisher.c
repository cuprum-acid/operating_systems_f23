#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 
#define MAX_SUBSCRIBERS 3
#define MAX_MESSAGE_SIZE 1024
 
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number of subscribers>\n", argv[0]);
        exit(1);
    }
 
    int n = atoi(argv[1]);
 
    if (n < 1 || n > MAX_SUBSCRIBERS) {
        fprintf(stderr, "Number of subscribers should be between 1 and %d.\n", MAX_SUBSCRIBERS);
        exit(1);
    }
 
    // Create named pipes for subscribers
    char pipe_name[20];
    for (int i = 1; i <= n; i++) {
        snprintf(pipe_name, sizeof(pipe_name), "/tmp/ex1/s%d", i);
        mkfifo(pipe_name, 0666);
    }
 
    // Open pipes for writing
    int pipe_fds[MAX_SUBSCRIBERS];
    for (int i = 1; i <= n; i++) {
        snprintf(pipe_name, sizeof(pipe_name), "/tmp/ex1/s%d", i);
        pipe_fds[i-1] = open(pipe_name, O_WRONLY);
    }
 
    char message[MAX_MESSAGE_SIZE];
    ssize_t bytes_read;
 
    while (1) {
        // Read message from stdin
        bytes_read = read(STDIN_FILENO, message, sizeof(message));
 
        if (bytes_read <= 0)
            break;
 
        // Publish message to all subscribers
        for (int i = 0; i < n; i++) {
            write(pipe_fds[i], message, bytes_read);
        }
    }
 
    // Close pipe file descriptors
    for (int i = 0; i < n; i++) {
        close(pipe_fds[i]);
    }
 
    return 0;
}
