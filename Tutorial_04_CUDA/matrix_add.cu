#include <iostream>
#include <cuda_runtime.h>

#define ROWS 4
#define COLS 4

__global__ void matrixAdd(int *A, int *B, int *C) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    if (row < ROWS && col < COLS) {
        int index = row * COLS + col;
        C[index] = A[index] + B[index];
    }
}

int main() {
    int A[ROWS * COLS];
    int B[ROWS * COLS];
    int C[ROWS * COLS];

    for (int i = 0; i < ROWS * COLS; i++) {
        A[i] = i + 1;
        B[i] = (i + 1) * 10;
    }

    int *d_A, *d_B, *d_C;
    int size = ROWS * COLS * sizeof(int);

    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    dim3 block(2, 2);
    dim3 grid(2, 2);

    matrixAdd<<<grid, block>>>(d_A, d_B, d_C);
    cudaDeviceSynchronize();

    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    std::cout << "Matrix A:\n";
    for (int i = 0; i < ROWS * COLS; i++) {
        std::cout << A[i] << (i % COLS == COLS - 1 ? "\n" : "\t");
    }

    std::cout << "\nMatrix B:\n";
    for (int i = 0; i < ROWS * COLS; i++) {
        std::cout << B[i] << (i % COLS == COLS - 1 ? "\n" : "\t");
    }

    std::cout << "\nMatrix C = A + B:\n";
    for (int i = 0; i < ROWS * COLS; i++) {
        std::cout << C[i] << (i % COLS == COLS - 1 ? "\n" : "\t");
    }

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}
