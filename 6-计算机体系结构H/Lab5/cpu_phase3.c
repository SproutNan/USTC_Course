#include <immintrin.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void gemm_baseline(float *A, float *B, float *C, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int k = 0; k < N; k++)
        {
            float a = A[i * N + k];
            for (int j = 0; j < N; j++)
            {
                C[i * N + j] += a * B[k * N + j];
            }
        }
    }
}

void gemm_avx_block(float *A, float *B, float *C, int si, int sj, int sk, int N, int BLOCK_SIZE)
{
    __m256 a, b, c;
    for (int i = si; i < si + BLOCK_SIZE; i++)
    {
        for (int j = sj; j < sj + BLOCK_SIZE; j++)
        {
            a = _mm256_set1_ps(A[i * N + j]);
            for (int k = sk; k < sk + BLOCK_SIZE; k += 8)
            {
                b = _mm256_loadu_ps(&B[j * N + k]);
                c = _mm256_loadu_ps(&C[i * N + k]);
                c = _mm256_fmadd_ps(a, b, c);
                _mm256_storeu_ps(&C[i * N + k], c);
            }
        }
    }
}

void gemm_verify(float *A, float *B, float *C, int N)
{
    float *C_baseline = (float *)malloc(N * N * sizeof(float));
    gemm_baseline(A, B, C_baseline, N);
    for (int i = 0; i < N * N; i++)
    {
        float d = C[i] - C_baseline[i];
        if (d > 1e-3 || d < -1e-3)
        {
            printf("Error: C[%d] = %f, C_baseline[%d] = %f\n", i, C[i], i, C_baseline[i]);
            free(C_baseline);
            return;
        }
    }
    free(C_baseline);
    printf("Correct!\n");
}

int main(int argc, char const *argv[])
{
    int N = (1 << atoi(argv[1]));
    int BLOCK_SIZE = (1 << atoi(argv[2]));

    // malloc
    float *A = (float *)malloc(N * N * sizeof(float));
    float *B = (float *)malloc(N * N * sizeof(float));
    float *C = (float *)malloc(N * N * sizeof(float));

    // random initialize A, B
    srand(time(NULL));
    for (int i = 0; i < N * N; i++)
    {
        A[i] = rand() / (float)RAND_MAX;
        B[i] = rand() / (float)RAND_MAX;
    }

    // use AVX
    clock_t start_avx = clock();
    for (int i = 0; i < N; i += BLOCK_SIZE)
        for (int j = 0; j < N; j += BLOCK_SIZE)
            for (int k = 0; k < N; k += BLOCK_SIZE)
                gemm_avx_block(A, B, C, i, j, k, N, BLOCK_SIZE);
    clock_t end_avx = clock();
    double elapsed_time_avx = ((double)(end_avx - start_avx)) / CLOCKS_PER_SEC;
    printf("time: %f s\n", elapsed_time_avx);

    // measure correctness
    gemm_verify(A, B, C, N);

    // Free memory for A, B, C
    free(A);
    free(B);
    free(C);

    return 0;
}
