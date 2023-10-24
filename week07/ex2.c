#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>

int main() {
    int text_file = open("text.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (text_file == -1) {
        perror("Error opening or creating text.txt");
        exit(1);
    }

    int random_file = open("/dev/random", O_RDONLY);
    if (random_file == -1) {
        perror("Error opening /dev/random");
        close(text_file);
        exit(1);
    }

    char c;
    off_t file_size = 0;
    long page_size = sysconf(_SC_PAGESIZE);
    char *chunk;

    while (file_size < 500 * 1024 * 1024) {
        if (file_size % (1024 * 1024) == 0) {
            chunk = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, text_file, file_size);
            if (chunk == MAP_FAILED) {
                perror("Error mapping memory");
                close(text_file);
                close(random_file);
                exit(1);
            }
        }


        if (read(random_file, &c, 1) == 1) {
            if (isprint(c)) {
                chunk[file_size % page_size] = c;
                file_size++;
            }
        }
    }

    munmap(chunk, page_size);

    close(text_file);
    close(random_file);

    text_file = open("text.txt", O_RDWR);
    if (text_file == -1) {
        perror("Error opening text.txt");
        exit(1);
    }

    char *text_data = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, text_file, 0);
    if (text_data == MAP_FAILED) {
        perror("Error mapping text.txt");
        close(text_file);
        exit(1);
    }

    int capital_count = 0;

    for (off_t i = 0; i < file_size; i++) {
        if (isupper(text_data[i])) {
            capital_count++;
            text_data[i] = tolower(text_data[i]);
        }
    }

    munmap(text_data, file_size);

    printf("Total capital letters: %d\n", capital_count);

    close(text_file);

    return 0;
}

