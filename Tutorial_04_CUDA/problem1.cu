#include <iostream>
#include <cuda_runtime.h>

__global__ void exploreThreads() {
    printf("Block %d, Thread %d | "
           "Block Size = %d, Grid Size = %d\n",
           blockIdx.x, threadIdx.x,
           blockDim.x, gridDim.x);
}

int main() {
    int threadsPerBlock = 4;
    int numberOfBlocks = 3;

    exploreThreads<<<numberOfBlocks, threadsPerBlock>>>();

    cudaDeviceSynchronize();

    return 0;
}
