#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TRIALS 3

// Independent dummy computation
double do_computation(long iterations) {
    double result = 0.0;

    for (long i = 0; i < iterations; i++) {
        result += (double)(i % 100) * 0.001;
    }

    return result;
}

int main(int argc, char **argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0)
            printf("Run with exactly 2 processes.\n");

        MPI_Finalize();
        return 1;
    }

    int messageSizesMB[] = {1, 10, 50};
    long computationIterations = 2000000;

    for (int test = 0; test < 3; test++) {
        int count = messageSizesMB[test] * 1000000 / sizeof(int);
        size_t bytes = (size_t)count * sizeof(int);

        int *buffer = malloc(bytes);

        if (buffer == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        if (rank == 0) {
            for (int i = 0; i < count; i++)
                buffer[i] = i;
        }

        double totalTime = 0.0;

        for (int trial = 0; trial < TRIALS; trial++) {
            MPI_Request request;
            double computationResult = 0.0;
            int acknowledgement = 1;

            MPI_Barrier(MPI_COMM_WORLD);

            if (rank == 0) {
                double start = MPI_Wtime();

                MPI_Isend(buffer, count, MPI_INT, 1, 0,
                          MPI_COMM_WORLD, &request);

                // Independent work while the send is outstanding
                computationResult =
                    do_computation(computationIterations);

                MPI_Wait(&request, MPI_STATUS_IGNORE);

                // Ensure the receiver has completed MPI_Wait too
                MPI_Recv(&acknowledgement, 1, MPI_INT, 1, 1,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                totalTime += MPI_Wtime() - start;
            } else {
                MPI_Irecv(buffer, count, MPI_INT, 0, 0,
                          MPI_COMM_WORLD, &request);

                // Independent work while the receive is outstanding
                computationResult =
                    do_computation(computationIterations);

                MPI_Wait(&request, MPI_STATUS_IGNORE);

                if (trial == 0) {
                    printf("Received %d MB: first = %d, last = %d\n",
                           messageSizesMB[test],
                           buffer[0], buffer[count - 1]);
                }

                MPI_Send(&acknowledgement, 1, MPI_INT, 0, 1,
                         MPI_COMM_WORLD);
            }

            // Keep the computation result observable
            if (computationResult < 0.0)
                printf("Computation result: %f\n",
                       computationResult);
        }

        if (rank == 0) {
            printf("Non-blocking average for %d MB: %.6f seconds\n\n",
                   messageSizesMB[test], totalTime / TRIALS);
        }

        free(buffer);
    }

    MPI_Finalize();
    return 0;
}
