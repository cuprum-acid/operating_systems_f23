#include <stdio.h>
#include <stdlib.h>

int const_tri(int *p, int n) {
    if (n == 0) return 0;
    if (n == 1 || n == 2) return 1;

    for (int i = 3; i <= n; ++i) {
        p[i % 3] = p[0] + p[1] + p[2];
    }

    return p[n % 3];
}


int main() {
    const int x = 1;
    const int *q;
    q = &x;
    int *p = (int *) malloc(3 * sizeof(int));

    p[0] = *q;
    p[1] = *q;
    p[2] = 2 * *q;

    printf("Memory Addresses:\n");
    printf("Cell 0: %p\n", (void *) &p[0]);
    printf("Cell 1: %p\n", (void *) &p[1]);
    printf("Cell 2: %p\n", (void *) &p[2]);

    if (&p[1] - &p[0] == 1 && &p[2] - &p[1] == 1) {
        printf("Cells are contiguous.\n");
    } else {
        printf("Cells are not contiguous.\n");
    }

    int n = 5;

    printf("Tribonacci(%d) = %d\n", n, const_tri(p, n));


    free(p);

    return 0;
}
