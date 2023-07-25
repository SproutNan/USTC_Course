#include <time.h>
#include <stdlib.h>
#include <stdio.h>

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

int main(int argc, char const *argv[])
{
    int N = (1 << atoi(argv[1]));

    float *A, *B, *C;
    // malloc
    A = (float *)malloc(N * N * sizeof(float));
    B = (float *)malloc(N * N * sizeof(float));
    C = (float *)malloc(N * N * sizeof(float));

    // random initialize A, B
    srand(time(NULL));
    for (int i = 0; i < N * N; i++)
    {
        A[i] = rand() / (float)RAND_MAX;
        B[i] = rand() / (float)RAND_MAX;
    }

    // measure time
    clock_t start, end;
    start = clock();
    gemm_baseline(A, B, C, N);
    end = clock();
    printf("Time: %f s\n", (double)(end - start) / CLOCKS_PER_SEC);

    // free
    free(A);
    free(B);
    free(C);

    return 0;
}
