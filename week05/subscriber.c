#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 
#define MAX_MESSAGE_SIZE 1024
 
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <subscriber index>\n", argv[0]);
        exit(1);
    }
 
    int subscriber_index = atoi(argv[1]);
 
    if (subscriber_index <= 0) {
        fprintf(stderr, "Subscriber index should be a positive integer.\n");
        exit(1);
    }
 
    char pipe_name[20];
    snprintf(pipe_name, sizeof(pipe_name), "/tmp/ex1/s%d", subscriber_index);
 
    int pipe_fd = open(pipe_name, O_RDONLY);
 
    if (pipe_fd == -1) {
        perror("Failed to open pipe");
        exit(1);
    }
 
    char message[MAX_MESSAGE_SIZE];
    ssize_t bytes_read;
 
    while (1) {
        bytes_read = read(pipe_fd, message, sizeof(message));
 
        if (bytes_read <= 0)
            break;
 
        // Print the received message to stdout
        write(STDOUT_FILENO, message, bytes_read);
    }
 
    close(pipe_fd);
 
    return 0;
}
