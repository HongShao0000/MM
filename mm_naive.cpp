#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>

void matrixMultiply(float **A, float **B, float **C, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void blockMatrixMultiply(float **A, float **B, float **C, int N, int blockSize) {
    for (int i = 0; i < N; i += blockSize) {
        for (int j = 0; j < N; j += blockSize) {
            for (int k = 0; k < N; k += blockSize) {
                for (int ii = i; ii < i + blockSize; ii++) {
                    for (int jj = j; jj < j + blockSize; jj++) {
                        C[ii][jj] = 0;
                        for (int kk = k; kk < k + blockSize; kk++) {
                            C[ii][jj] += A[ii][kk] * B[kk][jj];
                        }
                    }
                }
            }
        }
    }
}

void testMatrixMultiplicationPerformance(int N, int blockSize) {
    float **A, **B, **C;
    A = (float **)malloc(N * sizeof(float *));
    B = (float **)malloc(N * sizeof(float *));
    C = (float **)malloc(N * sizeof(float *));
    
    for (int i = 0; i < N; i++) {
        A[i] = (float *)malloc(N * sizeof(float));
        B[i] = (float *)malloc(N * sizeof(float));
        C[i] = (float *)malloc(N * sizeof(float));
    }

    // Initialize matrices A and B with random values
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = (float)rand() / RAND_MAX;
            B[i][j] = (float)rand() / RAND_MAX;
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    matrixMultiply(A, B, C, N);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    printf("Matrix multiplication without blocking: %f seconds\n", elapsed_time.count());

    start = std::chrono::high_resolution_clock::now();
    blockMatrixMultiply(A, B, C, N, blockSize);
    end = std::chrono::high_resolution_clock::now();
    elapsed_time = end - start;
    printf("Matrix multiplication with blocking: %f seconds\n", elapsed_time.count());

    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
}

int main() {
    int N = 1000; // Size of the matrices
    int blockSize = 64; // Block size for block matrix multiplication
    srand(time(NULL)); // Seed the random number generator

    testMatrixMultiplicationPerformance(N, blockSize);

    return 0;
}
