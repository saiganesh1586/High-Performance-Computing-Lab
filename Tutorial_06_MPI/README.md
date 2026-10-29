# HPC Lab: MPI Programming Tutorial

## 1. Introduction

This tutorial demonstrates parallel programming using the Message Passing Interface (MPI). The programs were compiled and executed using Open MPI in GitHub Codespaces.

The experiments cover process ranks, point-to-point communication, blocking and non-blocking operations, ring communication, and a two-dimensional process grid.

## 2. Environment

- Platform: GitHub Codespaces (Ubuntu/Linux)
- Programming language: C
- MPI implementation: Open MPI
- Compiler: `mpicc.openmpi`
- Execution command: `mpirun.openmpi --oversubscribe`

## 3. Problem 1: MPI Hello World

**Objective:** Display each process's rank and the total number of processes.

**Concepts used:**
- `MPI_Init()` initializes MPI.
- `MPI_Comm_rank()` obtains the current process rank.
- `MPI_Comm_size()` obtains the total process count.
- `MPI_Finalize()` ends MPI execution.

**Compile and run:**

```bash
mpicc.openmpi hello_mpi.c -o hello_mpi_openmpi
mpirun.openmpi --oversubscribe -np 2 ./hello_mpi_openmpi
mpirun.openmpi --oversubscribe -np 4 ./hello_mpi_openmpi
```

**Observed result:** The two-process execution reported ranks 0 and 1, with a process count of 2. The four-process execution reported ranks 0, 1, 2, and 3, with a process count of 4. Output order can vary.

## 4. Problem 2: Blocking Point-to-Point Communication

**Objective:** Transfer integer arrays from process 0 to process 1 using `MPI_Send()` and `MPI_Recv()`.

The experiment tests message sizes of 1 MB, 10 MB, and 50 MB. The receiving process prints the first and last array elements. `MPI_Wtime()` is used to measure elapsed time.

**Compile and run:**

```bash
mpicc.openmpi problem2_blocking.c -o problem2_blocking
mpirun.openmpi --oversubscribe -np 2 ./problem2_blocking
```

**Observed timing results:**

| Message size | Average transfer time |
|---:|---:|
| 1 MB | 0.000996 seconds |
| 10 MB | 0.004516 seconds |
| 50 MB | 0.021275 seconds |

**Analysis:** The measured transfer time increased as message size increased. Larger arrays require more data to be transferred and processed. Actual timings also depend on the MPI implementation, memory operations, and system load.

A blocking send may wait for the MPI implementation to make the send buffer safe to reuse. Its return does not necessarily mean that the receiving application has finished processing the message.

## 5. Problem 3: Non-Blocking Communication

**Objective:** Demonstrate non-blocking communication using `MPI_Isend()`, `MPI_Irecv()`, and `MPI_Wait()`.

Non-blocking operations allow the program to perform independent computation while communication is outstanding. `MPI_Wait()` is used to ensure the relevant operation has completed before its buffer is reused or accessed.

**Compile and run:**

```bash
mpicc.openmpi problem3_nonblocking.c -o problem3_nonblocking
mpirun.openmpi --oversubscribe -np 2 ./problem3_nonblocking
```

**Observed timing results:**

| Message size | Average non-blocking time |
|---:|---:|
| 1 MB | 0.009048 seconds |
| 10 MB | 0.012563 seconds |
| 50 MB | 0.033840 seconds |

**Analysis:** The measured non-blocking times were higher than the blocking times in these runs. Non-blocking communication does not guarantee better performance. The overhead of the operations and the amount of independent computation affect the outcome.

For a fair performance comparison, both implementations should use comparable workloads and timing boundaries. The current measurements should be interpreted in the context of the specific implementations tested.

## 6. Blocking vs. Non-Blocking Comparison

| Message size | Blocking (seconds) | Non-blocking (seconds) |
|---:|---:|---:|
| 1 MB | 0.000996 | 0.009048 |
| 10 MB | 0.004516 | 0.012563 |
| 50 MB | 0.021275 | 0.033840 |

The graph in `graphs/communication_time.png` plots message size against the measured average time for both implementations.

The measurements show increasing time with increasing message size for both versions. These results describe this experiment and should not be generalized to all MPI programs.

## 7. Problem 4: Ring Topology

**Objective:** Exchange integer values between neighboring processes in a ring.

Each process calculates its neighbors:

- Next: `(rank + 1) % size`
- Previous: `(rank - 1 + size) % size`

The program uses `MPI_Sendrecv()` to send its rank to the next process and receive a value from the previous process.

**Compile and run:**

```bash
mpicc.openmpi problem4_ring.c -o problem4_ring
mpirun.openmpi --oversubscribe -np 4 ./problem4_ring
```

**Observed result:** With four processes, process 0 received value 3, process 1 received value 0, process 2 received value 1, and process 3 received value 2.

`MPI_Sendrecv()` combines sending and receiving in one operation and helps avoid common deadlocks caused by incompatible blocking send/receive order.

## 8. Problem 5: Two-Dimensional Process Grid

**Objective:** Arrange four processes in a 2 × 2 grid and exchange messages with valid neighbors.

The program uses `MPI_Cart_create()` to create a Cartesian communicator, `MPI_Cart_coords()` to obtain grid coordinates, and `MPI_Cart_shift()` to find neighboring processes.

For a 2 × 2 grid:

```text
Process 0 ---- Process 1
    |               |
Process 2 ---- Process 3
```

**Compile and run:**

```bash
mpicc.openmpi problem5_grid.c -o problem5_grid
mpirun.openmpi --oversubscribe -np 4 ./problem5_grid
```

**Observed result:** The processes successfully exchanged messages with their valid up, down, left, and right neighbors. Boundary directions without a neighbor were ignored.

## 9. Files in This Tutorial

- `hello_mpi.c` — Problem 1
- `problem2_blocking.c` — Problem 2
- `problem3_nonblocking.c` — Problem 3
- `problem4_ring.c` — Problem 4
- `problem5_grid.c` — Problem 5
- `results/timing_results.csv` — measured timing data
- `graphs/plot_timings.py` — graph generation script
- `graphs/communication_time.png` — timing comparison graph

## 10. Conclusion

The tutorial demonstrates basic MPI initialization, rank and size discovery, point-to-point message passing, non-blocking communication, ring topology, and Cartesian process grids.

The timing experiments show that transfer time increased with message size. In these particular measurements, the non-blocking implementation took longer than the blocking implementation. Further controlled experiments with equivalent workloads would be needed to evaluate communication overlap and performance more conclusively.
