#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

bool is_prime(int n) {
    if (n <= 1) return false;
    int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

typedef struct prime_request {
    int a, b;
    int count;
} prime_request;

void *prime_counter(void *arg) {
    prime_request *req = (prime_request *)arg;
    int count = 0;
    for (int i = req->a; i < req->b; i++) {
        if (is_prime(i))
            count++;
    }
    req->count = count;
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s n m\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t threads[m];
    prime_request requests[m];
    int subinterval_size = n / m;
    int prime_count = 0;

    // Create threads and distribute work
    for (int i = 0; i < m; i++) {
        requests[i].a = i * subinterval_size;
        requests[i].b = (i == m - 1) ? n : (i + 1) * subinterval_size;
        pthread_create(&threads[i], NULL, prime_counter, &requests[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
        prime_count += requests[i].count;
    }

    printf("Number of primes in [0, %d) = %d\n", n, prime_count);

    return 0;
}

