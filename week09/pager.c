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

enum PageReplacementAlgorithm {
    RANDOM,
    NFU,
    AGING
};

// Function to handle SIGUSR1 signal from MMU
void handle_sigusr1(int signum) {

    int page_to_evict = -1;
    switch (page_replacement_algorithm) {
        case RANDOM:
            page_to_evict = random(page_table);
            break;
        case NFU:
            page_to_evict = nfu(page_table);
            break;
        case AGING:
            page_to_evict = aging(page_table);
            break;
    }

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

int random(struct PTE* page_table) {
    // Implement the random page replacement algorithm here
    int page_to_evict;
    do {
        page_to_evict = rand() % NUM_PAGES;
    } while (page_table[page_to_evict].valid);  // Choose a random page not currently in RAM
    return page_to_evict;
}


int nfu(struct PTE* page_table) {
    // Implement the NFU page replacement algorithm here
    int page_to_evict = 0;
    int min_counter = page_table[0].referenced;

    for (int i = 1; i < NUM_PAGES; i++) {
        if (page_table[i].valid && page_table[i].referenced < min_counter) {
            page_to_evict = i;
            min_counter = page_table[i].referenced;
        }
    }

    return page_to_evict;
}


int aging(struct PTE* page_table) {
    // Implement the Aging page replacement algorithm here
    int page_to_evict = 0;
    int min_counter = page_table[0].referenced;

    for (int i = 1; i < NUM_PAGES; i++) {
        // Shift the referenced bit to the right (aging)
        page_table[i].referenced >>= 1;

        if (page_table[i].valid && page_table[i].referenced < min_counter) {
            page_to_evict = i;
            min_counter = page_table[i].referenced;
        }
    }

    return page_to_evict;
}


int main(int argc, char* argv[]) {

    if (argc < 4) {
        fprintf(stderr, "Usage: %s <num_pages> <algorithm> <disk_accesses>\n", argv[0]);
        return 1;
    }

    int num_pages = atoi(argv[1]);
    char* algorithm = argv[2];
    disk_accesses = atoi(argv[3]);

    // Determine the page replacement algorithm based on the command-line argument
    enum PageReplacementAlgorithm page_replacement_algorithm;
    if (strcmp(algorithm, "random") == 0) {
        page_replacement_algorithm = RANDOM;
    } else if (strcmp(algorithm, "nfu") == 0) {
        page_replacement_algorithm = NFU;
    } else if (strcmp(algorithm, "aging") == 0) {
        page_replacement_algorithm = AGING;
    } else {
        fprintf(stderr, "Invalid algorithm specified\n");
        return 1;
    }

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