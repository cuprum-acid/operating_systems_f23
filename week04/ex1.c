#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    pid_t pid1, pid2;
    clock_t start_time, end_time;

    printf("Main process: PID=%d, Parent PID=%d\n", getpid(), getppid());

    // Create the first child process
    pid1 = fork();
    if (pid1 < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid1 == 0) {
        start_time = clock();
        printf("Child 1: PID=%d, Parent PID=%d\n", getpid(), getppid());
        end_time = clock();
        double execution_time_ms = (double)(end_time - start_time) * 1000 / CLOCKS_PER_SEC;
        printf("Child 1 execution time: %.2f ms\n", execution_time_ms);
        return 0;
    }

    // Create the second child process
    pid2 = fork();
    if (pid2 < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid2 == 0) {
        start_time = clock();
        printf("Child 2: PID=%d, Parent PID=%d\n", getpid(), getppid());
        end_time = clock();
        double execution_time_ms = (double)(end_time - start_time) * 1000 / CLOCKS_PER_SEC;
        printf("Child 2 execution time: %.2f ms\n", execution_time_ms);
        return 0;
    }
    
    // Wait for both child processes to complete
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}

