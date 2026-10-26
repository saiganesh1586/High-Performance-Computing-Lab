#include <iostream>
#include <cuda_runtime.h>

#define WIDTH 512
#define HEIGHT 512
#define INCREASE 50

__global__ void increaseBrightness(unsigned char *image) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < WIDTH && y < HEIGHT) {
        int index = y * WIDTH + x;
        int value = image[index] + INCREASE;

        if (value > 255)
            value = 255;

        image[index] = value;
    }
}

int main() {
    int size = WIDTH * HEIGHT * sizeof(unsigned char);

    unsigned char *image = new unsigned char[WIDTH * HEIGHT];

    // Initialize every pixel to brightness 100
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        image[i] = 100;
    }

    unsigned char *d_image;

    cudaMalloc(&d_image, size);
    cudaMemcpy(d_image, image, size, cudaMemcpyHostToDevice);

    dim3 block(16, 16);
    dim3 grid(32, 32);

    increaseBrightness<<<grid, block>>>(d_image);
    cudaDeviceSynchronize();

    cudaMemcpy(image, d_image, size, cudaMemcpyDeviceToHost);

    std::cout << "Image dimensions: "
              << WIDTH << " x " << HEIGHT << "\n";
    std::cout << "Blocks: "
              << grid.x << " x " << grid.y << "\n";
    std::cout << "Threads per block: "
              << block.x << " x " << block.y << "\n";
    std::cout << "Total pixels processed: "
              << WIDTH * HEIGHT << "\n";
    std::cout << "Original brightness: 100\n";
    std::cout << "New brightness: "
              << (int)image[0] << "\n";

    cudaFree(d_image);
    delete[] image;

    return 0;
}
