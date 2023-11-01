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
        // Set the referenced field
        page_table[page_to_load].referenced = getpid();
        printf("Read Request for page %d\n", page_to_load);
        printf("It is not a valid page --> page fault\n");
        printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
        // Signal Pager to handle the page fault
        kill(pid_pager, SIGUSR1);
        // Wait for Pager to signal back (SIGCONT) to indicate the page is loaded
        pause();
        printf("MMU resumed by SIGCONT signal from pager\n");
        // Print the updated page table
        for (int i = 0; i < NUM_PAGES; i++) {
            printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
        }
    }
}

int main(int argc, char* argv[]) {
    // Parse command line arguments and initialize the page table and other data structures
    int num_pages = atoi(argv[1]);
    char* reference_string = argv[2];
    pid_pager = atoi(argv[3]);

    // Initialize the page table
    for (int i = 0; i < num_pages; i++) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;
    }

    printf("MMU process\n");

    // Process the memory access requests and manage the page table
    int reference_string_length = strlen(reference_string);
    for (int i = 0; i < reference_string_length; i += 2) {
        char mode = reference_string[i]; // 'R' for read, 'W' for write
        int page_number = reference_string[i + 1] - '0'; // Convert char to int

        if (mode == 'R') {
            // Handle read request
            printf("Read Request for page %d\n", page_number);
            // Check if the page is in RAM (valid)
            if (page_table[page_number].valid) {
                // Page is in RAM
                printf("It is a valid page\n");
            } else {
                // Page is not in RAM (page fault)
                printf("It is not a valid page --> page fault\n");
                // Ask Pager to load it from disk (SIGUSR1 signal) and wait
                kill(pid_pager, SIGUSR1);
                // Wait for Pager to signal back (SIGCONT) to indicate the page is loaded
                pause();
                printf("MMU resumed by SIGCONT signal from pager\n");
                // Print the updated page table
                for (int i = 0; i < NUM_PAGES; i++) {
                    printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
                }
            }
        } else if (mode == 'W') {
            // Handle write request
            printf("Write Request for page %d\n", page_number);
            // Check if the page is in RAM (valid)
            if (page_table[page_number].valid) {
                // Page is in RAM
                printf("It is a valid page\n");
                // Set the dirty field
                page_table[page_number].dirty = true;
                // Print the updated page table
                for (int i = 0; i < NUM_PAGES; i++) {
                    printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
                }
            } else {
                // Page is not in RAM (page fault)
                printf("It is not a valid page --> page fault\n");
                // Ask Pager to load it from disk (SIGUSR1 signal) and wait
                kill(pid_pager, SIGUSR1);
                // Wait for Pager to signal back (SIGCONT) to indicate the page is loaded
                pause();
                printf("MMU resumed by SIGCONT signal from pager\n");
                // Print the updated page table
                for (int i = 0; i < NUM_PAGES; i++) {
                    printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
                }
            }
        }
    }

    // Signal the Pager process one last time
    printf("Done all requests.\n");
    printf("MMU sends SIGUSR1 to the pager.\n");
    kill(pid_pager, SIGUSR1);


    return 0;
}

