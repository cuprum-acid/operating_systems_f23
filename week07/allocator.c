#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 
#define MEMORY_SIZE 10000000
 
unsigned int memory[MEMORY_SIZE];
 
void initialize_memory() {
    memset(memory, 0, sizeof(memory));
}
 
int allocate_first_fit(unsigned int adrs, int size) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        int contiguous = 0;
        while (i < MEMORY_SIZE && memory[i] == 0) {
            contiguous++;
            if (contiguous == size) {
                for (int j = i - size + 1; j <= i; j++) {
                    memory[j] = adrs;
                }
                return i - size + 1;
            }
            i++;
        }
    }
    return -1;
}
 
 
int allocate_best_fit(unsigned int adrs, int size) {
    int best_fit_start = -1;
    int best_fit_size = MEMORY_SIZE + 1;
 
    int current_start = -1;
    int current_size = 0;
 
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            if (current_start == -1) {
                current_start = i;
            }
            current_size++;
        } else {
            if (current_size >= size && current_size < best_fit_size) {
                best_fit_start = current_start;
                best_fit_size = current_size;
            }
            current_start = -1;
            current_size = 0;
        }
    }
 
    if (current_size >= size && current_size < best_fit_size) {
        best_fit_start = current_start;
    }
 
    if (best_fit_start != -1) {
        for (int i = best_fit_start; i < best_fit_start + size; i++) {
            memory[i] = adrs;
        }
        return best_fit_start;
    }
    
    return -1;
}
 
 
int allocate_worst_fit(unsigned int adrs, int size) {
    int worst_fit_start = -1;
    int worst_fit_size = -1;
 
    int current_start = -1;
    int current_size = 0;
 
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            if (current_start == -1) {
                current_start = i;
            }
            current_size++;
        } else {
            if (current_size > worst_fit_size) {
                worst_fit_start = current_start;
                worst_fit_size = current_size;
            }
            current_start = -1;
            current_size = 0;
        }
    }
 
    if (current_size > worst_fit_size) {
        worst_fit_start = current_start;
    }
 
    if (worst_fit_start != -1) {
        for (int i = worst_fit_start; i < worst_fit_start + size; i++) {
            memory[i] = adrs;
        }
        return worst_fit_start;
    }
 
    return -1;
}
 
 
void clear(unsigned int adrs) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == adrs) {
            memory[i] = 0;
        }
    }
}
 
int main() {
    initialize_memory();
    clock_t start_time, end_time;
    FILE* file = fopen("queries.txt", "r");
    if (file == NULL) {
        printf("Error opening queries.txt\n");
        return 1;
    }

    unsigned int total_queries = 0;
    start_time = clock();

    char operation[10];
    unsigned int adrs, size;

    while (fscanf(file, "%s", operation) != EOF) {
        if (strcmp(operation, "allocate") == 0) {
            fscanf(file, "%u %u", &adrs, &size);
            allocate_first_fit(adrs, size);
        } else if (strcmp(operation, "clear") == 0) {
            fscanf(file, "%u", &adrs);
            clear(adrs);
        } else if (strcmp(operation, "end") == 0) {
            break;
        }
        total_queries++;
    }

    end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("First Fit Throughput: %lf queries per second\n", total_queries / execution_time);

    fclose(file);


    initialize_memory();
    file = fopen("queries.txt", "r");

    total_queries = 0;
    start_time = clock();

    while (fscanf(file, "%s", operation) != EOF) {
        if (strcmp(operation, "allocate") == 0) {
            fscanf(file, "%u %u", &adrs, &size);
            allocate_best_fit(adrs, size);
        } else if (strcmp(operation, "clear") == 0) {
            fscanf(file, "%u", &adrs);
            clear(adrs);
        } else if (strcmp(operation, "end") == 0) {
            break;
        }
        total_queries++;
    }

    end_time = clock();
    execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Best Fit Throughput: %lf queries per second\n", total_queries / execution_time);

    fclose(file);


    initialize_memory();
    file = fopen("queries.txt", "r");

    total_queries = 0;
    start_time = clock();

    while (fscanf(file, "%s", operation) != EOF) {
        if (strcmp(operation, "allocate") == 0) {
            fscanf(file, "%u %u", &adrs, &size);
            allocate_worst_fit(adrs, size);
        } else if (strcmp(operation, "clear") == 0) {
            fscanf(file, "%u", &adrs);
            clear(adrs);
        } else if (strcmp(operation, "end") == 0) {
            break;
        }
        total_queries++;
    }

    end_time = clock();
    execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Worst Fit Throughput: %lf queries per second\n", total_queries / execution_time);

    fclose(file);
    return 0;
}

