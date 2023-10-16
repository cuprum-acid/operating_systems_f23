#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
 
void sigusr1_handler(int signum) {
    char buffer[1024];
    int file_descriptor = open("text.txt", O_RDONLY);
    ssize_t bytes_read;
    while ((bytes_read = read(file_descriptor, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }
    close(file_descriptor);
}
 
void sigusr2_handler(int signum) {
    printf("Process terminating...\n");
    exit(0);
}
 
void sigterm_handler(int signum) {
    printf("Process terminating...\n");
    exit(0);
}

// Run program in sudo mode: (e.g. sudo ./agent)
int main() {
    FILE* pid_file = fopen("/var/run/agent.pid", "w");
    if (pid_file == NULL) {
        perror("Error creating agent.pid file");
        exit(1);
    }
    fprintf(pid_file, "%d", getpid());
    fclose(pid_file);
 
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);
    signal(SIGTERM, sigterm_handler);
 
    char buffer[1024];
    int file_descriptor = open("text.txt", O_RDONLY);
    ssize_t bytes_read;
    while ((bytes_read = read(file_descriptor, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }
    close(file_descriptor);
 
    while (1) {
        sleep(1);
    }
 
    return 0;
}
