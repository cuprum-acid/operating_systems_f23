#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#define DEVICE_FILE "/dev/input/by-path/platform-i8042-serio-0-event-kbd"

// Function to check for specific key combinations
void checkShortcuts(int keys[], int num_keys, int *pressed, const char *message) {
    int i, match = 1;
    for (i = 0; i < num_keys; ++i) {
        if (!pressed[keys[i]]) {
            match = 0;
            break;
        }
    }

    if (match) {
        printf("%s\n", message);
        FILE *output_file = fopen("ex1.txt", "a"); // Open file in append mode
        if (output_file == NULL) {
            perror("Error opening output file");
            exit(EXIT_FAILURE);
        }
        fprintf(output_file, "%s\n", message);
        fclose(output_file);

        for (i = 0; i < num_keys; ++i) {
            pressed[keys[i]] = 0;
        }
    }
}

int main() {
    int fd;
    struct input_event ev;
    int pressed[KEY_MAX] = {0}; // Array to keep track of pressed keys

    if ((fd = open(DEVICE_FILE, O_RDONLY)) == -1) {
        perror("Error opening device file");
        return EXIT_FAILURE;
    }

    FILE *output_file = fopen("ex1.txt", "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        return EXIT_FAILURE;
    }

    printf("Listening for keyboard events. Press 'E' and 'X' together to exit.\n");
    printf("Available shortcuts:\n");
    printf("P+E - I passed the Exam!\n");
    printf("C+A+P - Get some cappuccino!\n");
    printf("O+S - OS is cool!\n");

    while (1) {
        if (read(fd, &ev, sizeof(struct input_event)) == -1) {
            perror("Error reading");
            break;
        }

        if (ev.type == EV_KEY && (ev.value == 0 || ev.value == 1 || ev.value == 2)) {
            fprintf(output_file, "%s 0x%04x (%d)\n",
                    ev.value == 0 ? "RELEASED" : (ev.value == 1 ? "PRESSED" : "REPEATED"),
                    ev.code, ev.code);
            printf("%s 0x%04x (%d)\n",
                   ev.value == 0 ? "RELEASED" : (ev.value == 1 ? "PRESSED" : "REPEATED"),
                   ev.code, ev.code);

            // Keep track of pressed keys
            pressed[ev.code] = ev.value;

            // Check for shortcuts
            int pe_combo[] = {KEY_P, KEY_E};
            int cap_combo[] = {KEY_C, KEY_A, KEY_P};
            int os_combo[] = {KEY_O, KEY_S};

            checkShortcuts(pe_combo, 2, pressed, "I passed the Exam!");
            checkShortcuts(cap_combo, 3, pressed, "Get some cappuccino!");
            checkShortcuts(os_combo, 2, pressed, "OS is cool!");

            // Exiting condition for 'E' and 'X' pressed together
            if (pressed[KEY_E] && pressed[KEY_X]) {
                break;
            }
        }
    }

    fclose(output_file);
    close(fd);
    return EXIT_SUCCESS;
}

