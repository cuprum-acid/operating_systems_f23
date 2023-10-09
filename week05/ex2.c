#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// Define the struct Thread
struct Thread {
    pthread_t id;        // Thread ID
    int i;               // Index of the thread
    char message[256];   // Message
};

// Function that each thread will execute
void *thread_function(void *arg) {
    struct Thread *thread_data = (struct Thread *)arg;
    printf("Thread %d is created\n", thread_data->i);
    sprintf(thread_data->message, "Hello from thread %d", thread_data->i);
    printf("Thread %d: %s\n", thread_data->i, thread_data->message);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("Please provide a positive integer for the number of threads.\n");
        return 1;
    }

    struct Thread threads[n];

    for (int i = 0; i < n; i++) {
        threads[i].i = i;
        if (pthread_create(&threads[i].id, NULL, thread_function, &threads[i]) != 0) {
            perror("Thread creation failed");
            return 1;
        }
        pthread_join(threads[i].id, NULL);
    }

    return 0;
}

