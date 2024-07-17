
#include <cuda_runtime.h>

const int BUFFER_SIZE = 1024 * 1024 * 1024;
const int NUM_BLOCKS = 1024;
const int NUM_THREADS = 1024;
const int NUM_STREAMS = 16;

__global__ void kernel(int *buffer, int size) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < size) {
        buffer[i] = i;
    }
}

void cuda_calls() {
    int *buffer;
    cudaMalloc(&buffer, BUFFER_SIZE);

    cudaStream_t streams[NUM_STREAMS];
    for (int i = 0; i < NUM_STREAMS; i++) {
        cudaStreamCreate(&streams[i]);
    }

    for (int i = 0; i < NUM_STREAMS; i++) {
        kernel<<<NUM_BLOCKS, NUM_THREADS, 0, streams[i]>>>(buffer, BUFFER_SIZE);
    }

    for (int i = 0; i < NUM_STREAMS; i++) {
        cudaStreamSynchronize(streams[i]);
    }

    for (int i = 0; i < NUM_STREAMS; i++) {
        cudaStreamDestroy(streams[i]);
    }

    cudaFree(buffer);
}