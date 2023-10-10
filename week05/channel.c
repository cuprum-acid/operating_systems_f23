#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 1024

int main() {
    int pipe_fd[2]; // File descriptors for the pipe
    pid_t child_pid;

    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Create a child process
    child_pid = fork();

    if (child_pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // This is the subscriber process
        close(pipe_fd[1]); // Close the write end of the pipe

        char message[MAX_MESSAGE_SIZE];
        int bytes_read;

        while ((bytes_read = read(pipe_fd[0], message, MAX_MESSAGE_SIZE)) > 0) {
            write(STDOUT_FILENO, message, bytes_read); // Print the message to stdout
        }

        close(pipe_fd[0]);
    } else {
        // This is the publisher process
        close(pipe_fd[0]); // Close the read end of the pipe

        char message[MAX_MESSAGE_SIZE];
        int bytes_read;

        while (1) {
            // Read a message from stdin
            bytes_read = read(STDIN_FILENO, message, MAX_MESSAGE_SIZE);

            if (bytes_read <= 0) {
                break; // Exit if stdin is closed or an error occurs
            }

            // Write the message to the pipe
            write(pipe_fd[1], message, bytes_read);
        }

        close(pipe_fd[1]); // Close the write end of the pipe
    }

    return 0;
}

