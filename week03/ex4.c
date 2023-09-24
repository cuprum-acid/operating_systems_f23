#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

int typeOfData = 0;

void *aggregate(void *base, size_t size, int n, void *initial_value, void *(*opr)(const void *, const void *)) {
    if (size == sizeof(int)) {
        typeOfData = 1;
    } else if (size == sizeof(double)) {
        typeOfData = 2;
    }
    char *current = (char *) base;
    void *result = initial_value;

    for (int i = 0; i < n; i++) {
        result = opr(result, current);
        current += size;
    }

    return result;
}

void *add(const void *a, const void *b) {
    void *result = malloc(sizeof(double));
    *(double *) result = *(double *) a + *(double *) b;
    return result;
}

void *multiply(const void *a, const void *b) {
    if (typeOfData == 1) {
        void *result = malloc(sizeof(int));
        *(int *) result = *(int *) a * *(int *) b;
        return result;
    } else if (typeOfData == 2) {
        void *result = malloc(sizeof(double));
        *(double *) result = *(double *) a * *(double *) b;
        return result;
    }
}

void *max(const void *a, const void *b) {
    if (typeOfData == 1) {
        if (*(int *) a > *(int *) b) {
            void *result = malloc(sizeof(int));
            *(int *) result = *(int *) a;
            return result;
        } else {
            void *result = malloc(sizeof(int));
            *(int *) result = *(int *) b;
            return result;
        }
    } else if (typeOfData == 2) {
        if (*(double *) a > *(double *) b) {
            void *result = malloc(sizeof(double));
            *(double *) result = *(double *) a;
            return result;
        } else {
            void *result = malloc(sizeof(double));
            *(double *) result = *(double *) b;
            return result;
        }
    }
}

int main() {

    double doubleArray[] = {14.2, 3.4, 5.6, 7.8, 9.0};
    int intArray[] = {2, 28, 3, 4, 5};

    double initialDouble = 0.0;
    int initialInt = 0;
    double initialDoubleP = 1.0;
    int initialIntP = 1;
    double initialDoubleM = DBL_MIN;
    int initialIntM = INT_MIN;

    void *doubleSum = aggregate(doubleArray, sizeof(double), 5, &initialDouble, add);
    void *doubleProduct = aggregate(doubleArray, sizeof(double), 5, &initialDoubleP, multiply);
    void *doubleMax = aggregate(doubleArray, sizeof(double), 5, &initialDoubleM, max);

    printf("Double Array Sum: %.2lf\n", *(double *) doubleSum);
    printf("Double Array Product: %.2lf\n", *(double *) doubleProduct);
    printf("Double Array Max: %.2lf\n", *(double *) doubleMax);

    free(doubleSum);
    free(doubleProduct);
    free(doubleMax);

    void *intSum = aggregate(intArray, sizeof(int), 5, &initialInt, add);
    void *intProduct = aggregate(intArray, sizeof(int), 5, &initialIntP, multiply);
    void *intMax = aggregate(intArray, sizeof(int), 5, &initialIntM, max);

    printf("Integer Array Sum: %d\n", *(int *) intSum);
    printf("Integer Array Product: %d\n", *(int *) intProduct);
    printf("Integer Array Max: %d\n", *(int *) intMax);

    free(intSum);
    free(intProduct);
    free(intMax);

    return 0;
}
