
#include <stdio.h>
#include <omp.h>

int main(void) {
    int v[3] = {0, 0, 0};
    int indices[8] = {0, 1, 0, 2, 1, 0, 2, 0};
    int n = 8;

    // Version 1: No protection
    #pragma omp parallel for shared(v, indices, n)
    for (int i = 0; i < n; i++) {
        v[indices[i]] += i + 1;
    }

    printf("Without protection: %d %d %d\n",
           v[0], v[1], v[2]);

    // Reset array
    v[0] = v[1] = v[2] = 0;

    // Version 2: Critical section
    #pragma omp parallel for shared(v, indices, n)
    for (int i = 0; i < n; i++) {
        #pragma omp critical
        {
            v[indices[i]] += i + 1;
        }
    }

    printf("With critical: %d %d %d\n",
           v[0], v[1], v[2]);

    return 0;
}
