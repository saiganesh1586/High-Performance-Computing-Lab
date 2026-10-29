#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TRIALS 3

int main(int argc, char **argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) {
            printf("Please run with exactly 2 processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    // Message sizes in decimal megabytes
    int messageSizesMB[] = {1, 10, 50};
    int numberOfTests = 3;

    for (int test = 0; test < numberOfTests; test++) {
        int count = messageSizesMB[test] * 1000000 / sizeof(int);

        int *buffer = (int *)malloc((size_t)count * sizeof(int));

        if (buffer == NULL) {
            printf("Process %d: Memory allocation failed.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        double totalTime = 0.0;

        for (int trial = 0; trial < TRIALS; trial++) {
            if (rank == 0) {
                // Initialize the array
                for (int i = 0; i < count; i++) {
                    buffer[i] = i;
                }
            }

            MPI_Barrier(MPI_COMM_WORLD);

            if (rank == 0) {
                int acknowledgement;
                double start = MPI_Wtime();

                // Send the array to Process 1
                MPI_Send(buffer, count, MPI_INT, 1, 0,
                         MPI_COMM_WORLD);

                // Wait until Process 1 confirms receipt
                MPI_Recv(&acknowledgement, 1, MPI_INT, 1, 1,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                double elapsed = MPI_Wtime() - start;
                totalTime += elapsed;

                if (trial == 0) {
                    printf("Message size: %d MB\n",
                           messageSizesMB[test]);
                    printf("First element: %d\n", buffer[0]);
                    printf("Last element: %d\n", buffer[count - 1]);
                }
            } else {
                int acknowledgement = 1;

                // Receive the array from Process 0
                MPI_Recv(buffer, count, MPI_INT, 0, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                if (trial == 0) {
                    printf("Process 1 received %d MB: "
                           "first = %d, last = %d\n",
                           messageSizesMB[test],
                           buffer[0], buffer[count - 1]);
                }

                // Tell Process 0 that receipt is complete
                MPI_Send(&acknowledgement, 1, MPI_INT, 0, 1,
                         MPI_COMM_WORLD);
            }
        }

        if (rank == 0) {
            printf("Average transfer time for %d MB: %.6f seconds\n\n",
                   messageSizesMB[test], totalTime / TRIALS);
        }

        free(buffer);
    }

    MPI_Finalize();
    return 0;
}
