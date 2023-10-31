#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

int main() {
    struct rusage usage;
    long mb = 10 * 1024 * 1024;  // 10 MB
    // long mb = 1024 * 1024 * 1024;  // 1024 MB
    int seconds = 10;

    while (seconds > 0) {
        void *ptr = malloc(mb);
        if (ptr == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            return 1;
        }

        memset(ptr, 0, mb);

        getrusage(RUSAGE_SELF, &usage);
        printf("Memory usage (MB): %ld\n", usage.ru_maxrss / 1024);

        free(ptr);

        sleep(1);
        seconds--;
    }

    return 0;
}

