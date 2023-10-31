#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PASSWORD_LENGTH 13

char generatePrintableChar() {
    char printableChar;
    do {
        int random_fd = open("/dev/random", O_RDONLY);
        if (random_fd == -1) {
            perror("Failed to open /dev/random");
            exit(1);
        }

        if (read(random_fd, &printableChar, 1) != 1) {
            perror("Failed to read from /dev/random");
            close(random_fd);
            exit(1);
        }

        close(random_fd);
    } while (printableChar < 32 || printableChar > 126); // Ensure it's a printable character

    return printableChar;
}

int main() {
    FILE *pid_file = fopen("/tmp/ex1.pid", "w");
    if (pid_file != NULL) {
        fprintf(pid_file, "%d\n", getpid());
        fclose(pid_file);
    } else {
        perror("Failed to open /tmp/ex1.pid");
        exit(1);
    }

    char password[PASSWORD_LENGTH + 1];
    password[0] = 'p';
    password[1] = 'a';
    password[2] = 's';
    password[3] = 's';
    password[4] = ':';
    for (int i = 5; i < PASSWORD_LENGTH; i++) {
        password[i] = generatePrintableChar();
    }
    password[PASSWORD_LENGTH] = '\0'; 

    // Print the password for debugging
    printf("Generated Password: %s\n", password);

    char *shared_memory = mmap(NULL, PASSWORD_LENGTH + 1, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (shared_memory == MAP_FAILED) {
        perror("Failed to create shared memory mapping");
        exit(1);
    }

    strcpy(shared_memory, password);

    while (1) {
        sleep(1);
    }

    return 0;
}

