
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

int main(void) {
    long long samples[] = {
        10, 100, 10000, 1000000,
        10000000, 100000000, 1000000000
    };

    int count = sizeof(samples) / sizeof(samples[0]);
    const double actual_pi = 3.14159265358979323846;

    FILE *fp = fopen("pi_results.csv", "w");

    if (fp == NULL) {
        printf("Error creating output file.\n");
        return 1;
    }

    fprintf(fp, "Points,EstimatedPi,AbsoluteError,TimeSeconds\n");

    for (int s = 0; s < count; s++) {
        long long n = samples[s];
        long long inside = 0;

        double start = omp_get_wtime();

        #pragma omp parallel reduction(+:inside)
        {
            unsigned int seed =
                12345u + (unsigned int)omp_get_thread_num();

            #pragma omp for schedule(static)
            for (long long i = 0; i < n; i++) {
                double x = 2.0 * rand_r(&seed) /
                           ((double)RAND_MAX + 1.0) - 1.0;
                double y = 2.0 * rand_r(&seed) /
                           ((double)RAND_MAX + 1.0) - 1.0;

                if (x*x + y*y <= 1.0)
                    inside++;
            }
        }

        double pi = 4.0 * (double)inside / (double)n;
        double error = fabs(pi - actual_pi);
        double elapsed = omp_get_wtime() - start;

        printf("N=%lld, pi=%.10f, error=%.10f, time=%.3f s\n",
               n, pi, error, elapsed);

        fprintf(fp, "%lld,%.10f,%.10f,%.6f\n",
                n, pi, error, elapsed);
        fflush(fp);
    }

    fclose(fp);
    printf("\nResults saved in pi_results.csv\n");

    return 0;
}
