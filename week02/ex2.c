#include <stdio.h>

int main() {
    char input[256];
    char character;
    int length = 0;

    printf("Enter a string character-by-character (terminate with '.' or Enter):\n");

    while (1) {
        character = getchar();

        if (character == '.' || character == '\n') {
            break;
        }

        if (length < 255) {
            input[length] = character;
            length++;
        } else {
            printf("Maximum string length reached.\n");
            break;
        }
    }

    input[length] = '\0';

    printf("\"");
    for (int i = length - 1; i >= 0; i--) {
        putchar(input[i]);
    }
    printf("\"\n");

    return 0;
}