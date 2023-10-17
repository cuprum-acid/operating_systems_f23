#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <limits.h>
 
#define PS_MAX 10
 
typedef struct {
    int idx;
    int at, bt, rt, wt, ct, tat, burst, quantum;
} ProcessData;
 
int running_process = -1;
unsigned total_time = 1;
ProcessData data[PS_MAX];
pid_t ps[PS_MAX];
unsigned data_size;
 
void read_file(FILE* file) {
    char line[100];
 
    int count = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        int idx, at, bt;
        char* endptr;
        idx = (int)strtol(line, &endptr, 10);
        if (endptr == line) {
            continue;
        }
        at = (int)strtol(endptr, &endptr, 10);
        if (endptr == line) {
            continue;
        }
        bt = (int)strtol(endptr, &endptr, 10);
        if (endptr == line) {
            continue;
        }
 
        data[count].idx = idx;
        data[count].at = at;
        data[count].bt = bt;
        data[count].rt = bt;
        data[count].burst = bt;
        data[count].wt = 0;
        data[count].ct = 0;
        data[count].tat = 0;
        count++;
    }
 
    data_size = count;
}
 
 
void resume(pid_t process) {
    if (process >= 0 && process < PS_MAX && ps[process] > 0) {
        kill(ps[process], SIGCONT);
    }
}
 
void suspend(pid_t process) {
    if (process >= 0 && process < PS_MAX && ps[process] > 0) {
        kill(ps[process], SIGTSTP);
    }
}
 
void terminate(pid_t process) {
    if (process >= 0 && process < PS_MAX && ps[process] > 0) {
        kill(ps[process], SIGTERM);
        waitpid(ps[process], NULL, 0);
        ps[process] = 0;
    }
}
 
void create_process(int new_process) {
    if (running_process != -1) {
        suspend(running_process);
    }
 
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
 
    if (child_pid == 0) {
        char process_idx_str[2];
        sprintf(process_idx_str, "%d", new_process);
 
        char* worker_args[] = {"./worker", process_idx_str, NULL};
 
        execvp(worker_args[0], worker_args);
        perror("Exec failed");
        exit(EXIT_FAILURE);
    } else {
        ps[new_process] = child_pid;
        running_process = new_process;
        printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", new_process, data[new_process].burst);
    }
}
 
ProcessData find_next_process() {
    int next_process_idx = -1;

    for (int i = 0; i < data_size; i++) {
        if (data[i].at <= total_time && data[i].burst > 0) {
            next_process_idx = i;
            break;
        }
    }

    if (next_process_idx == -1) {
        printf("Scheduler: Runtime: %u seconds.\nNo eligible process found.\n", total_time);
        total_time++;
    }

    return data[next_process_idx];
}

 
void report() {
    printf("Simulation results.....\n");
    int sum_wt = 0;
    int sum_tat = 0;
 
    for (int i = 0; i < data_size; i++) {
        printf("process %d: \n", i);
        printf("	at=%d\n", data[i].at);
        printf("	bt=%d\n", data[i].bt);
        printf("	ct=%d\n", data[i].ct);
        printf("	wt=%d\n", data[i].wt);
        printf("	tat=%d\n", data[i].tat);
        printf("	rt=%d\n", data[i].rt);
        sum_wt += data[i].wt;
        sum_tat += data[i].tat;
    }
 
    printf("data size = %d\n", data_size);
    float avg_wt = (float)sum_wt / data_size;
    float avg_tat = (float)sum_tat / data_size;
    printf("Average results for this run:\n");
    printf("	avg_wt=%f\n", avg_wt);
    printf("	avg_tat=%f\n", avg_tat);
}
 
void check_burst() {
    for (int i = 0; i < data_size; i++) {
        if (data[i].burst > 0) {
            return;
        }
    }
 
    report();
 
    exit(EXIT_SUCCESS);
}
 
void schedule_handler(int signum) {
    total_time++;

    if (running_process != -1) {
        data[running_process].burst--;

        printf("Scheduler: Runtime: %u seconds\n", total_time);
        printf("Process %d is running with %d seconds left\n", running_process, data[running_process].burst);

        if (data[running_process].burst == 0 || data[running_process].quantum == 0) {
            data[running_process].ct = total_time;
            data[running_process].tat = data[running_process].ct - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;

            printf("Scheduler: Terminating Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            terminate(running_process);
        } else {
            data[running_process].quantum--;
        }
    }

    check_burst();

    ProcessData next_process = find_next_process();

    if (next_process.idx != running_process) {
        if (running_process != -1) {
            printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            suspend(running_process);
        }

        running_process = next_process.idx;

        if (ps[running_process] == 0) {
            create_process(running_process);
            data[running_process].rt = total_time - data[running_process].at;
        } else {
            printf("Scheduler: Resuming Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            resume(running_process);
        }
    }
}

 
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <data_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
 
    FILE* in_file = fopen(argv[1], "r");
    if (in_file == NULL) {
        printf("File is not found or cannot open it!\n");
        exit(EXIT_FAILURE);
    } else {
        read_file(in_file);
    }
    
    printf("Enter the quantum value: ");
    int quantum;
    scanf("%d", &quantum);
    
    data[running_process].quantum = quantum;
 
    struct itimerval timer;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
 
    signal(SIGALRM, schedule_handler);
 
    while (1);
}
