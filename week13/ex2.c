#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int processes, resources;
int available[MAX_RESOURCES];
int maximum[MAX_PROCESSES][MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];

void input_data(FILE *file) {
    fscanf(file, "%d", &resources);

    for (int i = 0; i < resources; ++i)
        fscanf(file, "%d", &available[i]);

    fscanf(file, "%d", &processes);

    for (int i = 0; i < processes; ++i)
        for (int j = 0; j < resources; ++j)
            fscanf(file, "%d", &maximum[i][j]);

    for (int i = 0; i < processes; ++i)
        for (int j = 0; j < resources; ++j)
            fscanf(file, "%d", &allocation[i][j]);
}

bool is_safe_state() {
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = { false };

    for (int i = 0; i < resources; ++i)
        work[i] = available[i];

    int count = 0;
    while (count < processes) {
        bool found = false;
        for (int i = 0; i < processes; ++i) {
            if (!finish[i]) {
                bool possible = true;
                for (int j = 0; j < resources; ++j) {
                    if (need[i][j] > work[j]) {
                        possible = false;
                        break;
                    }
                }
                if (possible) {
                    for (int j = 0; j < resources; ++j)
                        work[j] += allocation[i][j];
                    finish[i] = true;
                    found = true;
                    count++;
                }
            }
        }
        if (!found)
            return false;
    }
    return true;
}

void detect_deadlock() {
    for (int i = 0; i < processes; ++i)
        for (int j = 0; j < resources; ++j)
            need[i][j] = maximum[i][j] - allocation[i][j];

    if (is_safe_state())
        printf("No deadlock detected.\n");
    else {
        printf("Processes involved in deadlock:\n");
        for (int i = 0; i < processes; ++i) {
            if (!is_safe_state() && need[i][0] != -1) {
                printf("Process %d\n", i);
                for (int j = 0; j < resources; ++j)
                    need[i][j] = -1;
            }
        }
    }
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    input_data(file);
    fclose(file);

    detect_deadlock();

    return 0;
}

