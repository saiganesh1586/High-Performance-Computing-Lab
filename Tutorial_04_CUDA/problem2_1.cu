#include <iostream>
#include <cuda_runtime.h>

__global__ void mapThreads2D() {
    int globalX = blockIdx.x * blockDim.x + threadIdx.x;
    int globalY = blockIdx.y * blockDim.y + threadIdx.y;

    printf("Global position: (%d, %d) | "
           "Block: (%d, %d) | Thread: (%d, %d)\n",
           globalX, globalY,
           blockIdx.x, blockIdx.y,
           threadIdx.x, threadIdx.y);
}

int main() {
    dim3 threadsPerBlock(2, 2);
    dim3 numberOfBlocks(2, 2);

    mapThreads2D<<<numberOfBlocks, threadsPerBlock>>>();

    cudaDeviceSynchronize();

    return 0;
}
