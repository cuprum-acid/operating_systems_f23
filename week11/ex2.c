#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
 
struct inode {
    char name[16];
    int size;
    int blockPointers[8];
    int used;
};
 
struct inode inodes[16]; // Array to hold inodes
char freeBlockList[128]; // Free block list
 
void initialize() {
    memset(freeBlockList, 0, sizeof(freeBlockList)); // Set all blocks as free initially
    for (int i = 0; i < 16; ++i) {
        inodes[i].used = 0; // Mark all inodes as free initially
    }
}
 
void writeSuperBlock(int fd) {
    lseek(fd, 0, SEEK_SET); // Move to the start of the disk
    write(fd, freeBlockList, 128); // Write free block list
    write(fd, inodes, sizeof(inodes)); // Write inodes
}
 
void readSuperBlock(int fd) {
    lseek(fd, 0, SEEK_SET); // Move to the start of the disk
    read(fd, freeBlockList, 128); // Read free block list
    read(fd, inodes, sizeof(inodes)); // Read inodes
}
 
void create(char name[16], int size) {
    // Check free space on the disk
    int freeBlocks = 0;
    for (int i = 0; i < 128; ++i) {
        if (freeBlockList[i] == 0) {
            freeBlocks++;
        }
    }
 
    if (freeBlocks < size) {
        printf("Not enough free space to create file %s\n", name);
        return;
    }
 
    // Locate a free inode
    int freeInodeIndex = -1;
    for (int i = 0; i < 16; ++i) {
        if (inodes[i].used == 0) {
            freeInodeIndex = i;
            break;
        }
    }
 
    if (freeInodeIndex == -1) {
        printf("No free inode available to create file %s\n", name);
        return;
    }
 
    // Allocate data blocks to the file
    int blocksAllocated = 0;
    for (int i = 0; i < 128 && blocksAllocated < size; ++i) {
        if (freeBlockList[i] == 0) {
            freeBlockList[i] = 1; // Mark block as in-use
            inodes[freeInodeIndex].blockPointers[blocksAllocated] = i; // Update block pointers
            blocksAllocated++;
        }
    }
 
    // Update inode details
    strncpy(inodes[freeInodeIndex].name, name, sizeof(inodes[freeInodeIndex].name));
    inodes[freeInodeIndex].size = size;
    inodes[freeInodeIndex].used = 1;
 
    printf("File %s created successfully\n", name);
}
 
void delete(char name[16]) {
    int found = 0;
    for (int i = 0; i < 16; ++i) {
        if (strcmp(inodes[i].name, name) == 0 && inodes[i].used == 1) {
            inodes[i].used = 0; // Mark inode as free
 
            // Free blocks associated with the file
            for (int j = 0; j < inodes[i].size; ++j) {
                freeBlockList[inodes[i].blockPointers[j]] = 0; // Mark blocks as free
            }
 
            printf("File %s deleted successfully\n", name);
            found = 1;
            break;
        }
    }
 
    if (!found) {
        printf("File %s not found\n", name);
    }
}
 
void ls() {
    for (int i = 0; i < 16; ++i) {
        if (inodes[i].used == 1) {
            printf("File: %s, Size: %d blocks\n", inodes[i].name, inodes[i].size);
        }
    }
}
 
void read_block(char name[16], int blockNum, char buf[1024]) {
    int found = 0;
    for (int i = 0; i < 16; ++i) {
        if (strcmp(inodes[i].name, name) == 0 && inodes[i].used == 1) {
            found = 1;
            if (blockNum >= 0 && blockNum < inodes[i].size) {
                int blockIndex = inodes[i].blockPointers[blockNum];
                // Read block content into buf (pseudo code)
                // lseek(fd, blockIndex * 1024, SEEK_SET);
                // read(fd, buf, 1024);
                printf("Reading block %d from file %s\n", blockNum, name);
            } else {
                printf("Invalid block number for file %s\n", name);
            }
            break;
        }
    }
 
    if (!found) {
        printf("File %s not found\n", name);
    }
}
 
void write_block(char name[16], int blockNum, char buf[1024]) {
    int found = 0;
    for (int i = 0; i < 16; ++i) {
        if (strcmp(inodes[i].name, name) == 0 && inodes[i].used == 1) {
            found = 1;
            if (blockNum >= 0 && blockNum < inodes[i].size) {
                int blockIndex = inodes[i].blockPointers[blockNum];
                // Write buf content to block (pseudo code)
                // lseek(fd, blockIndex * 1024, SEEK_SET);
                // write(fd, buf, 1024);
                printf("Writing to block %d in file %s\n", blockNum, name);
            } else {
                printf("Invalid block number for file %s\n", name);
            }
            break;
        }
    }
 
    if (!found) {
        printf("File %s not found\n", name);
    }
}
 
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
 
    initialize(); // Initialize free block list and inodes
 
    char diskName[20]; // To store the disk file name
    FILE *input_file = fopen(argv[1], "r");
 
    if (input_file == NULL) {
        fprintf(stderr, "Error opening input file.\n");
        exit(EXIT_FAILURE);
    }
 
    fscanf(input_file, "%s", diskName); // Read disk file name
 
    int disk_fd = open(diskName, O_RDWR);
    if (disk_fd < 0) {
        fprintf(stderr, "Error opening disk file.\n");
        exit(EXIT_FAILURE);
    }
 
    readSuperBlock(disk_fd); // Read super block information
 
    // Process commands from the input file
    char operation[3], fileName[16];
    int size, blockNum;
    char data[1024];
 
    while (fscanf(input_file, "%s", operation) != EOF) {
        if (strcmp(operation, "C") == 0) {
            fscanf(input_file, "%s %d", fileName, &size);
            create(fileName, size);
        } else if (strcmp(operation, "D") == 0) {
            fscanf(input_file, "%s", fileName);
            delete(fileName);
        } else if (strcmp(operation, "L") == 0) {
            ls();
        } else if (strcmp(operation, "R") == 0) {
            fscanf(input_file, "%s %d", fileName, &blockNum);
            read_block(fileName, blockNum, data);
        } else if (strcmp(operation, "W") == 0) {
            fscanf(input_file, "%s %d", fileName, &blockNum);
            // Read data into 'data' from input file or generate dummy content
            write_block(fileName, blockNum, data);
        }
    }
 
    writeSuperBlock(disk_fd); // Write updated super block information
    close(disk_fd);
    fclose(input_file);
    return 0;
}

