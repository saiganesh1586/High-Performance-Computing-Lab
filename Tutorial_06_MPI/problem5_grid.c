#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    int rank, size;
    int dims[2] = {2, 2};
    int periods[2] = {0, 0};
    int coords[2];
    int up, down, left, right;
    int received;
    MPI_Comm grid;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != 4) {
        if (rank == 0) {
            printf("Please run this program with exactly 4 processes.\n");
        }
        MPI_Finalize();
        return 0;
    }

    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &grid);

    MPI_Comm_rank(grid, &rank);
    MPI_Cart_coords(grid, rank, 2, coords);

    // Find vertical neighbors.
    MPI_Cart_shift(grid, 0, 1, &up, &down);

    // Find horizontal neighbors.
    MPI_Cart_shift(grid, 1, 1, &left, &right);

    printf("Process %d: row=%d, column=%d\n",
           rank, coords[0], coords[1]);

    // Send to the process below and receive from above.
    MPI_Sendrecv(&rank, 1, MPI_INT, down, 10,
                 &received, 1, MPI_INT, up, 10,
                 grid, MPI_STATUS_IGNORE);

    if (up != MPI_PROC_NULL) {
        printf("Process %d received %d from UP (rank %d)\n",
               rank, received, up);
    }

    // Send to the process above and receive from below.
    MPI_Sendrecv(&rank, 1, MPI_INT, up, 11,
                 &received, 1, MPI_INT, down, 11,
                 grid, MPI_STATUS_IGNORE);

    if (down != MPI_PROC_NULL) {
        printf("Process %d received %d from DOWN (rank %d)\n",
               rank, received, down);
    }

    // Send to the process on the right and receive from the left.
    MPI_Sendrecv(&rank, 1, MPI_INT, right, 12,
                 &received, 1, MPI_INT, left, 12,
                 grid, MPI_STATUS_IGNORE);

    if (left != MPI_PROC_NULL) {
        printf("Process %d received %d from LEFT (rank %d)\n",
               rank, received, left);
    }

    // Send to the process on the left and receive from the right.
    MPI_Sendrecv(&rank, 1, MPI_INT, left, 13,
                 &received, 1, MPI_INT, right, 13,
                 grid, MPI_STATUS_IGNORE);

    if (right != MPI_PROC_NULL) {
        printf("Process %d received %d from RIGHT (rank %d)\n",
               rank, received, right);
    }

    MPI_Comm_free(&grid);
    MPI_Finalize();

    return 0;
}
