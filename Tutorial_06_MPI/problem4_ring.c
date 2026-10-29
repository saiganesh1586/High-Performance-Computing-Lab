#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 4) {
        if (rank == 0)
            printf("Please run with at least 4 processes.\n");

        MPI_Finalize();
        return 1;
    }

    int next = (rank + 1) % size;
    int previous = (rank - 1 + size) % size;

    // Each process sends its rank to the next process
    // and receives the previous process's rank.
    int sendValue = rank;
    int receivedValue = -1;

    MPI_Sendrecv(
        &sendValue, 1, MPI_INT, next, 0,
        &receivedValue, 1, MPI_INT, previous, 0,
        MPI_COMM_WORLD, MPI_STATUS_IGNORE
    );

    printf("Process %d: previous = %d, next = %d, "
           "received value = %d\n",
           rank, previous, next, receivedValue);

    MPI_Finalize();
    return 0;
}
