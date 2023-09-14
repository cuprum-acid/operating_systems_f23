#include <stdio.h>

void convert(long long x, int s, int t) {
    if ((s < 2 || s > 10) || (t < 2 || t > 10)) {
        printf("cannot convert!\n");
        return;
    }

    long long decimalNumber = 0;
    long long base = 1;

    // Convert the number to decimal
    while (x > 0) {
        int digit = x % 10;
        if (digit >= s) {
            printf("cannot convert!\n");
            return;
        }
        decimalNumber += digit * base;
        x /= 10;
        base *= s;
    }

    // Convert the decimal number to the target base
    char result[50];
    int i = 0;
    while (decimalNumber > 0) {
        int remainder = decimalNumber % t;
        result[i++] = remainder + '0';
        decimalNumber /= t;
    }

    if (i == 0) {
        result[i++] = '0';
    }

    for (int j = i - 1; j >= 0; j--) {
        printf("%c", result[j]);
    }

    printf("\n");
}

int main() {
    long long number;
    int sourceBase, targetBase;

    printf("Enter a non-negative number: ");
    if (scanf("%lld", &number) != 1 || number < 0) {
        printf("Invalid input!\n");
        return 1;
    }

    printf("Enter the source numeral system (2-10): ");
    if (scanf("%d", &sourceBase) != 1) {
        printf("Invalid input!\n");
        return 1;
    }

    printf("Enter the target numeral system (2-10): ");
    if (scanf("%d", &targetBase) != 1) {
        printf("Invalid input!\n");
        return 1;
    }

    convert(number, sourceBase, targetBase);

    return 0;
}
