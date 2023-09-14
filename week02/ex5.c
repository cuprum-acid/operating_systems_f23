#include <stdio.h>

int tribonacci(int n) {
    if (n == 0) return 0;
    if (n == 1 || n == 2) return 1;

    int t0 = 0, t1 = 1, t2 = 1, tn;

    for (int i = 3; i <= n; i++) {
        tn = t0 + t1 + t2;
        t0 = t1;
        t1 = t2;
        t2 = tn;
    }

    return tn;
}

int main() {
    int n1 = 4, n2 = 36;

    int result1 = tribonacci(n1);
    int result2 = tribonacci(n2);

    printf("T%d = %d\n", n1, result1);
    printf("T%d = %d\n", n2, result2);

    return 0;
}