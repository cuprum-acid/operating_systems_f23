#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>

#define NUM_PAGES 4
#define NUM_FRAMES 2

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

struct PTE* page_table;
char RAM[NUM_FRAMES][8];
char disk[NUM_PAGES][8];
int disk_accesses = 0;
int free_frames = NUM_FRAMES;

// Function to handle SIGUSR1 signal from MMU
void handle_sigusr1(int signum) {
    // Find a page that is not in RAM (valid = 0)
    for (int i = 0; i < NUM_PAGES; i++) {
        if (!page_table[i].valid) {
            // If there are free frames, allocate one
            if (free_frames > 0) {
                for (int j = 0; j < NUM_FRAMES; j++) {
                    if (page_table[i].frame == -1) {
                        page_table[i].frame = j;
                        page_table[i].valid = true;
                        free_frames--;
                        break;
                    }
                }
            } else {
                // If there are no free frames, choose a random victim page
                int victim_page;
                do {
                    victim_page = rand() % NUM_PAGES;
                } while (page_table[victim_page].referenced == 0);
                
                if (page_table[victim_page].dirty) {
                    // Simulate writing the page to disk
                    strcpy(disk[victim_page], RAM[page_table[victim_page].frame]);
                    disk_accesses++;
                }
                // Update page table for the victim page
                page_table[victim_page].valid = false;
                page_table[victim_page].frame = -1;
                page_table[victim_page].referenced = 0;
            }
            printf("Page %d is referenced\n", i);
            printf("We can allocate it to free frame %d\n", page_table[i].frame);
            // Simulate copying data from disk to RAM
            strcpy(RAM[page_table[i].frame], disk[i]);
            printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", i, page_table[i].frame);

            kill(signum, SIGCONT);
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    // Create and initialize the page table as a memory-mapped file
    int page_table_fd = open("/tmp/ex2/pagetable", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(page_table_fd, NUM_PAGES * sizeof(struct PTE));
    page_table = (struct PTE*)mmap(NULL, NUM_PAGES * sizeof(struct PTE), PROT_READ | PROT_WRITE, MAP_SHARED, page_table_fd, 0);
    close(page_table_fd);

    // Initialize the page table
    for (int i = 0; i < NUM_PAGES; i++) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;
    }

    // Initialize RAM
    for (int i = 0; i < NUM_FRAMES; i++) {
        memset(RAM[i], 0, sizeof(RAM[i]));
    }

    // Set up signal handler for SIGUSR1
    signal(SIGUSR1, handle_sigusr1);

    printf("Pager process\n");

    // Initialize the disk array with random data (for testing)
    srand(time(NULL));
    for (int i = 0; i < NUM_PAGES; i++) {
        for (int j = 0; j < 8; j++) {
            disk[i][j] = 'A' + rand() % 26;
        }
    }

    printf("%d disk accesses in total\n", disk_accesses);

    // Destroy the mapped file
    munmap(page_table, NUM_PAGES * sizeof(struct PTE));
    unlink("/tmp/ex2/pagetable");

    return 0;
}

