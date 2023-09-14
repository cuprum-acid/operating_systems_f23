#include <stdio.h>
#include <ctype.h>

int count(const char str[], char target) {
    int count = 0;
    char lowercaseTarget = tolower(target);

    for (int i = 0; str[i] != '\0'; i++) {
        char lowercaseChar = tolower(str[i]);
        if (lowercaseChar == lowercaseTarget) {
            count++;
        }
    }

    return count;
}

void countAll(const char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        char lowercaseChar = tolower(str[i]);
        if (isalpha(lowercaseChar)) {
            int charCount = count(str, lowercaseChar);
            printf("%c:%d, ", lowercaseChar, charCount);
        }
    }
    printf("\n");
}

int main() {
    char inputString[256];

    printf("Enter a string: ");
    scanf("%255s", inputString);

    countAll(inputString);

    return 0;
}
