#include "libmin.h"

#define N 64  // Matrix dimension (adjust for more cache stress)

static int A[N][N];
static int B[N][N];
static int C[N][N];      // Result from main multiplication (order: i, j, k)
static int refC[N][N];   // Reference result using an alternative loop order (order: i, k, j)

int main() {
    // Initialize random seed
    libmin_srand(12345);

    // Initialize matrices A and B with random data; zero out C and refC
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // Use modulo to restrict the range of random numbers (0..99)
            A[i][j] = libmin_rand() % 1000;
            B[i][j] = libmin_rand() % 1000;
            C[i][j] = 0;
            refC[i][j] = 0;
        }
    }

    // --- First Multiplication: Loop order (i, j, k) ---
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // --- Second Multiplication (Reference): Loop order (i, k, j) ---
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            for (int j = 0; j < N; j++) {
                refC[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // --- Verification: Compare C and refC ---
    int error = 0;
    for (int i = 0; i < N && !error; i++) {
        for (int j = 0; j < N && !error; j++) {
            if (C[i][j] != refC[i][j]) {
                libmin_printf("Mismatch at (%d, %d): computed %d, expected %d\n", 
                                i, j, C[i][j], refC[i][j]);
                error = 1;
            }
        }
    }

    // --- Final Reporting ---
    if (error) {
        // If an error is detected, print a failure message.
        libtarg_fail(1);
    } else {
        // Compute a checksum of the result matrix for quick verification.
        int checksum = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                checksum = (checksum + C[i][j]) % 100000;
            }
        }
        libmin_printf("Matrix multiplication successful. Checksum: %d\n", checksum);
        
        // Print the computed result matrix.
        libmin_printf("Result Matrix C:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                libmin_printf("%d ", C[i][j]);
            }
            libmin_printf("\n");
        }
        
        libtarg_success();
    }
    return 0;
}

