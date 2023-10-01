#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define VECTOR_SIZE 120

void generateRandomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100; 
    }
}

int calculatePartialDotProduct(int u[], int v[], int start, int end) {
    int result = 0;
    for (int i = start; i < end; i++) {
        result += u[i] * v[i];
    }
    return result;
}

int main() {
    srand(time(NULL));
        
    // Clear file from previous records
    FILE* temp = fopen("temp.txt", "w");
    fprintf(temp, "\n");
    fclose(temp);

    int u[VECTOR_SIZE];
    int v[VECTOR_SIZE];
    generateRandomVector(u, VECTOR_SIZE);
    generateRandomVector(v, VECTOR_SIZE);

    int n;
    printf("Enter the number of processes (n): ");
    scanf("%d", &n);

    if (n <= 0 || n % 2 != 0) {
        printf("Please enter a valid even number of processes.\n");
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("Fork failed");
            return 1;
        }

        if (child_pid == 0) {
            close(pipefd[0]);

            int start = i * VECTOR_SIZE / n;
            int end = (i + 1) * VECTOR_SIZE / n;

            int partialResult = calculatePartialDotProduct(u, v, start, end);

            FILE* file = fopen("temp.txt", "a");
            fprintf(file, "Process %d: Partial Result = %d\n", getpid(), partialResult);
            fclose(file);

            close(pipefd[1]);
            exit(0);
        }
    }

    close(pipefd[1]);

    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    int totalResult = 0;
    FILE* file = fopen("temp.txt", "r");
    if (file != NULL) {
        char line[100];
        while (fgets(line, sizeof(line), file)) {
            int partialResult;
            sscanf(line, "Process %*d: Partial Result = %d", &partialResult);
            totalResult += partialResult;
        }
        fclose(file);
    } else {
        perror("Error opening file temp.txt");
    }

    printf("Dot Product Result = %d\n", totalResult);

    return 0;
}

