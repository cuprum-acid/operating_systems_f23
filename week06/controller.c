#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int agent_pid;

void sigint_handler(int signum) {
    if (agent_pid > 0) {
        kill(agent_pid, SIGTERM);
    }
    exit(0);
}

int main() {
    struct stat stat_buffer;
    if (stat("/var/run/agent.pid", &stat_buffer) == -1) {
        perror("Error: No agent found.");
        exit(1);
    }

    FILE* pid_file = fopen("/var/run/agent.pid", "r");
    if (pid_file == NULL) {
        perror("Error reading agent PID.");
        exit(1);
    }
    fscanf(pid_file, "%d", &agent_pid);
    fclose(pid_file);

    printf("Agent found.\n");

    signal(SIGINT, sigint_handler);

    while (1) {
        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"}: ");
        char command[20];
        scanf("%s", command);

        if (strcmp(command, "read") == 0) {
            kill(agent_pid, SIGUSR1);
        } else if (strcmp(command, "exit") == 0) {
            kill(agent_pid, SIGUSR2);
            exit(0);
        } else if (strcmp(command, "stop") == 0) {
            kill(agent_pid, SIGSTOP);
        } else if (strcmp(command, "continue") == 0) {
            kill(agent_pid, SIGCONT);
        } else {
            printf("Invalid command.\n");
        }
    }

    return 0;
}

