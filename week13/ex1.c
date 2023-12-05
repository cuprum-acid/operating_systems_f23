#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_THREADS 100
#define MAX_RESOURCES 100

pthread_mutex_t mutex[MAX_RESOURCES];
pthread_t threads[MAX_THREADS];

int mutex_status[MAX_THREADS][MAX_RESOURCES];
int num_threads, num_resources;


bool visited[MAX_THREADS];
bool in_stack[MAX_THREADS];

bool isCyclicUtil(int v) {
    if (!visited[v]) {
        visited[v] = true;
        in_stack[v] = true;

        for (int i = 0; i < num_threads; ++i) {
            if (mutex_status[v][i]) {
                if (!visited[i] && isCyclicUtil(i)) {
                    return true;
                } else if (in_stack[i]) {
                    return true;
                }
            }
        }
    }
    in_stack[v] = false;
    return false;
}

bool isDeadlock() {
    for (int i = 0; i < num_threads; ++i) {
        visited[i] = false;
        in_stack[i] = false;
    }

    for (int i = 0; i < num_threads; ++i) {
        if (isCyclicUtil(i)) {
            return true;
        }
    }
    return false;
}

void check_deadlock() {
    printf("Checking for deadlock...\n");

    if (isDeadlock()) {
        printf("Deadlock detected!\n");
        printf("Status of threads and mutexes:\n");
        for (int i = 0; i < num_threads; ++i) {
            printf("Thread %d mutexes: ", i);
            for (int j = 0; j < num_resources; ++j) {
                if (mutex_status[i][j]) {
                    printf("%d ", j);
                }
            }
            printf("\n");
        }
        exit(EXIT_FAILURE);
    } else {
        printf("No deadlock detected.\n");
    }
}

void *start_routine(void *arg) {
    int *params = (int *)arg;
    int thread_id = params[0];
    int resource_id = params[1];

    printf("Thread %d is created\n", thread_id);
    printf("Mutexes locked by Thread %d: ", thread_id);
    for (int i = 0; i < num_resources; ++i) {
        if (mutex_status[thread_id][i]) {
            printf("%d ", i);
        }
    }
    printf("\n");

    printf("Thread %d tries to lock mutex %d\n", thread_id, resource_id);
    if (pthread_mutex_trylock(&mutex[resource_id]) == 0) {
        mutex_status[thread_id][resource_id] = 1;
        printf("Thread %d locked mutex %d\n", thread_id, resource_id);

        int sleep_time = rand() % 5;
        sleep(sleep_time);

        pthread_mutex_unlock(&mutex[resource_id]);
        mutex_status[thread_id][resource_id] = 0;
        printf("Thread %d unlocked mutex %d\n", thread_id, resource_id);

        int all_unlocked = 1;
        for (int i = 0; i < num_resources; ++i) {
            if (mutex_status[thread_id][i]) {
                all_unlocked = 0;
                break;
            }
        }
        if (all_unlocked) {
            printf("Thread %d has released all acquired mutexes. Terminating.\n", thread_id);
            pthread_exit(NULL);
        }
    } else {
        printf("Thread %d couldn't lock mutex %d\n", thread_id, resource_id);
    }

    return NULL;
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <num_resources> <num_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    num_resources = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    for (int i = 0; i < num_resources; ++i) {
        pthread_mutex_init(&mutex[i], NULL);
    }

    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error opening input file.\n");
        return EXIT_FAILURE;
    }

    int thread_id, resource_id;
    while (fscanf(file, "%d %d", &thread_id, &resource_id) != EOF) {
        int params[2] = {thread_id, resource_id};
        pthread_create(&threads[thread_id], NULL, start_routine, (void *)params);
    }

    fclose(file);

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    check_deadlock();

    for (int i = 0; i < num_resources; ++i) {
        pthread_mutex_destroy(&mutex[i]);
    }

    return EXIT_SUCCESS;
}

