#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    for (int i = 0; i < n; i++) {
        pid_t child_pid = fork();
        sleep(5);

        if (child_pid == -1) {
            perror("fork");
            exit(1);
        } else if (child_pid == 0) {
            printf("Child process %d created\n", getpid());
            exit(0);
        }
    }

    sleep(5 * n);
    return 0;
}

