#include <iostream>
#include <cuda_runtime.h>

int main() {
    int deviceCount = 0;
    cudaError_t error = cudaGetDeviceCount(&deviceCount);

    if (error != cudaSuccess) {
        std::cout << "CUDA Error: "
                  << cudaGetErrorString(error) << std::endl;
        return 1;
    }

    std::cout << "Number of CUDA devices: "
              << deviceCount << "\n\n";

    for (int i = 0; i < deviceCount; i++) {
        cudaDeviceProp prop;
        cudaGetDeviceProperties(&prop, i);

        std::cout << "Device " << i << "\n";
        std::cout << "Name: " << prop.name << "\n";
        std::cout << "Global Memory: "
                  << prop.totalGlobalMem / (1024.0 * 1024.0)
                  << " MB\n";
        std::cout << "Shared Memory per Block: "
                  << prop.sharedMemPerBlock / 1024.0
                  << " bytes\n";
        std::cout << "Multiprocessors: "
                  << prop.multiProcessorCount << "\n";
        std::cout << "Max Threads per Block: "
                  << prop.maxThreadsPerBlock << "\n";
        std::cout << "Max Grid Size: ("
                  << prop.maxGridSize[0] << ", "
                  << prop.maxGridSize[1] << ", "
                  << prop.maxGridSize[2] << ")\n";
        std::cout << "Compute Capability: "
                  << prop.major << "." << prop.minor << "\n";
        std::cout << "Warp Size: " << prop.warpSize << "\n\n";
    }

    return 0;
}
