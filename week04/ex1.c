#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void measureExecutionTime(clock_t start_time, const char* process_name) {
    clock_t end_time = clock();
    double execution_time_ms = (double)(end_time - start_time) * 1000 / CLOCKS_PER_SEC;
    printf("%s execution time: %.2f ms\n", process_name, execution_time_ms);
}

int main() {
    pid_t pid1, pid2;
    clock_t main_start_time, child1_start_time, child2_start_time;

    main_start_time = clock();

    printf("Main process: PID=%d, Parent PID=%d\n", getpid(), getppid());

    // Create the first child process
    pid1 = fork();
    if (pid1 < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid1 == 0) {
        child1_start_time = clock();
        printf("Child 1: PID=%d, Parent PID=%d\n", getpid(), getppid());
        measureExecutionTime(child1_start_time, "Child 1");
        return 0;
    }

    // Create the second child process
    pid2 = fork();
    if (pid2 < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid2 == 0) {
        child2_start_time = clock();
        printf("Child 2: PID=%d, Parent PID=%d\n", getpid(), getppid());
        measureExecutionTime(child2_start_time, "Child 2");
        return 0;
    }
    
    // Wait for both child processes to complete
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    measureExecutionTime(main_start_time, "Main process");

    return 0;
}

