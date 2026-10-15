
#include <stdio.h>
#include <omp.h>

#define N 1000

int main(void) {
    double start, end;

    omp_set_dynamic(0);
    omp_set_num_threads(2);

    // Test 1: Static scheduling
    start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp for schedule(static)
        for (int i = 1; i <= N; i++) {
            for (volatile int j = 0; j < i * 100; j++) {}
        }

        #pragma omp for schedule(static)
        for (int i = 1; i <= N; i++) {
            for (volatile int j = 0; j < (N - i + 1) * 100; j++) {}
        }
    }

    end = omp_get_wtime();
    printf("Static: %.6f seconds\n", end - start);

    // Test 2: Static, chunk size 1
    start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp for schedule(static, 1)
        for (int i = 1; i <= N; i++) {
            for (volatile int j = 0; j < i * 100; j++) {}
        }

        #pragma omp for schedule(static, 1)
        for (int i = 1; i <= N; i++) {
            for (volatile int j = 0; j < (N - i + 1) * 100; j++) {}
        }
    }

    end = omp_get_wtime();
    printf("Static,1: %.6f seconds\n", end - start);

    // Test 3: Dynamic, chunk size 1
    start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp for schedule(dynamic, 1)
        for (int i = 1; i <= N; i++) {
            for (volatile int j = 0; j < i * 100; j++) {}
        }

        #pragma omp for schedule(dynamic, 1)
        for (int i = 1; i <= N; i++) {
            for (volatile int j = 0; j < (N - i + 1) * 100; j++) {}
        }
    }

    end = omp_get_wtime();
    printf("Dynamic,1: %.6f seconds\n", end - start);

    return 0;
}
