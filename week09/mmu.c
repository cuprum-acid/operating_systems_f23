// mmu.c

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

#define NUM_PAGES 4
#define NUM_FRAMES 2

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

struct PTE page_table[NUM_PAGES];
char RAM[NUM_FRAMES][8];
char disk[NUM_PAGES][8];
int pid_pager;

int hits = 0;
int misses = 0;

// Function to handle SIGUSR1 signal from Pager
void handle_sigusr1(int signum) {
    int page_to_load = -1;
    for (int i = 0; i < NUM_PAGES; i++) {
        if (!page_table[i].valid) {
            page_to_load = i;
            break;
        }
    }
    if (page_to_load >= 0) {
        page_table[page_to_load].referenced = getpid();
        printf("Read Request for page %d\n", page_to_load);
        printf("It is not a valid page --> page fault\n");
        printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
        kill(pid_pager, SIGUSR1);
        pause();
        printf("MMU resumed by SIGCONT signal from pager\n");
        misses++;
        // Print the updated page table
        for (int i = 0; i < NUM_PAGES; i++) {
            printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
        }
    } else {
        // Page is in RAM (valid)
        printf("Read Request for page %d\n", page_to_load);
        printf("It is a valid page\n");
        hits++;
    }
}

int main(int argc, char* argv[]) {
    // Parse command line arguments and initialize the page table and other data structures
    int num_pages = atoi(argv[1]);
    char* reference_string = argv[2];
    pid_pager = atoi(argv[3]);

    for (int i = 0; i < num_pages; i++) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;
    }

    printf("MMU process\n");

    int reference_string_length = strlen(reference_string);
    for (int i = 0; i < reference_string_length; i += 2) {
        char mode = reference_string[i];
        int page_number = reference_string[i + 1] - '0';

        if (mode == 'R') {
            printf("Read Request for page %d\n", page_number);
            if (page_table[page_number].valid) {
                printf("It is a valid page\n");
                hits++;
            } else {
                printf("It is not a valid page --> page fault\n");
                kill(pid_pager, SIGUSR1);
                pause();
                printf("MMU resumed by SIGCONT signal from pager\n");
                misses++;
            }
        } else if (mode == 'W') {
            printf("Write Request for page %d\n", page_number);
            if (page_table[page_number].valid) {
                printf("It is a valid page\n");
                page_table[page_number].dirty = true;
                hits++;
            } else {
                printf("It is not a valid page --> page fault\n");
                kill(pid_pager, SIGUSR1);
                pause();
                printf("MMU resumed by SIGCONT signal from pager\n");
                misses++;
            }
        }
    }

    // Calculate and print the hit ratio
    float hit_ratio = (float)hits / (hits + misses);
    printf("Hit ratio: %.2f\n", hit_ratio);

    // Signal the Pager process one last time
    printf("Done all requests.\n");
    printf("MMU sends SIGUSR1 to the pager.\n");
    kill(pid_pager, SIGUSR1);

    return 0;
}
