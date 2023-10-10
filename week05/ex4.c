#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

// Function to check primality
bool is_prime(int n) {
    if (n <= 1) return false;
    int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

// The mutexes
pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t k_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t c_lock = PTHREAD_MUTEX_INITIALIZER;

int k = 0;
int c = 0;
int n = 0;

// Get the next prime candidate
int get_number_to_check() {
    int ret;
    pthread_mutex_lock(&k_lock);
    ret = k;
    if (k != n) {
        k++;
    }
    pthread_mutex_unlock(&k_lock);
    return ret;
}

// Increment the prime counter
void increment_primes() {
    pthread_mutex_lock(&c_lock);
    c++;
    pthread_mutex_unlock(&c_lock);
}

// Start routine
void *check_primes(void *arg) {
    while (1) {
        int num = get_number_to_check();
        if (num >= n) break; // All numbers checked
        if (is_prime(num)) {
            increment_primes();
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s n m\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t threads[m];

    // Create threads
    for (int i = 0; i < m; i++) {
        pthread_create(&threads[i], NULL, check_primes, NULL);
    }

    // Join threads
    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the final result
    printf("Number of primes in [0, %d) = %d\n", n, c);

    return 0;
}

