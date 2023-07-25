#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>
#include <cmath>
#define BLOCK_SIZE 16

using namespace std;

__device__ int dev_N;

__global__ void gemm_baseline(float *A, float *B, float *C)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;
    if (i < dev_N && j < dev_N)
    {
        C[j * dev_N + i] = 0;
        int b_idx = blockIdx.x * blockDim.x;
        for (int a_idx = blockIdx.y * blockDim.y * dev_N;
             a_idx < blockIdx.y * blockDim.y * dev_N + dev_N - 1; a_idx += blockDim.x, b_idx += blockDim.y * dev_N)
        {
            __shared__ float Sub_A[BLOCK_SIZE][BLOCK_SIZE];
            __shared__ float Sub_B[BLOCK_SIZE][BLOCK_SIZE];

            Sub_A[threadIdx.y][threadIdx.x] = A[a_idx + threadIdx.y * dev_N + threadIdx.x];
            Sub_B[threadIdx.y][threadIdx.x] = B[b_idx + threadIdx.y * dev_N + threadIdx.x];

            __syncthreads();

            for (int k = 0; k < BLOCK_SIZE; k++)
            {
                C[j * dev_N + i] += Sub_A[threadIdx.y][k] * Sub_B[k][threadIdx.x];
            }

            __syncthreads();
        }
    }
}

void gemm_verify(float *A, float *B, float *C, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            float d = 0;
            for (int k = 0; k < N; k++)
            {
                d += A[i * N + k] * B[k * N + j];
            }
            if (C[i * N + j] - d > 1e-3 || C[i * N + j] - d < -1e-3)
            {
                printf("Error: C[%d] = %f, C_baseline[%d] = %f\n", i * N + j, C[i], i * N + j, d);
                return;
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    int N = (1 << atoi(argv[1]));

    cudaMemcpyToSymbol(dev_N, &N, sizeof(int));

    // malloc
    float *A = (float *)malloc(N * N * sizeof(float));
    float *B = (float *)malloc(N * N * sizeof(float));
    float *C = (float *)malloc(N * N * sizeof(float));

    // random initialize A, B
    srand((unsigned int)time(0));
    for (int i = 0; i < N * N; i++)
    {
        A[i] = rand() / (float)RAND_MAX;
        B[i] = rand() / (float)RAND_MAX;
    }

    // cumalloc
    float *A_device, *B_device, *C_device;
    cudaMalloc((void **)&A_device, N * N * sizeof(float));
    cudaMalloc((void **)&B_device, N * N * sizeof(float));
    cudaMalloc((void **)&C_device, N * N * sizeof(float));
    cudaMemcpy(A_device, A, N * N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(B_device, B, N * N * sizeof(float), cudaMemcpyHostToDevice);

    // define gridsize & blocksize
    dim3 grid(N / BLOCK_SIZE, N / BLOCK_SIZE, 1), block(BLOCK_SIZE, BLOCK_SIZE, 1);

    // timing
    cudaEvent_t start, stop;

    // create CUDA events for timing
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // record start event
    cudaEventRecord(start, 0);

    // launch kernel
    gemm_baseline<<<grid, block>>>(A_device, B_device, C_device);

    // record stop event
    cudaEventRecord(stop, 0);

    // synchronize events
    cudaEventSynchronize(stop);

    // calculate elapsed time
    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);

    // print elapsed time
    printf("Time: %.3f ms\n", elapsedTime);

    cudaMemcpy(C, C_device, N * N * sizeof(float), cudaMemcpyDeviceToHost);
    gemm_verify(A, B, C, N);

    cudaFree(A_device);
    cudaFree(B_device);
    cudaFree(C_device);
    free(A);
    free(B);
    free(C);
}