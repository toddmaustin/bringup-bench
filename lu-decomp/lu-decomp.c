#include <stdio.h>

#define N 5  // Matrix size
// #define N 3  // Matrix size

void print_matrix(const char* name, double mat[N][N]) {
    printf("%s =\n", name);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%8.4f ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void lu_decomposition(double A[N][N], double L[N][N], double U[N][N]) {
    for (int i = 0; i < N; i++) {
        // Compute U row
        for (int k = i; k < N; k++) {
            double sum = 0;
            for (int j = 0; j < i; j++)
                sum += L[i][j] * U[j][k];
            U[i][k] = A[i][k] - sum;
        }

        // Compute L column
        for (int k = i; k < N; k++) {
            if (i == k)
                L[i][i] = 1.0;
            else {
                double sum = 0;
                for (int j = 0; j < i; j++)
                    sum += L[k][j] * U[j][i];
                L[k][i] = (A[k][i] - sum) / U[i][i];
            }
        }
    }
}

int main() {
    double A[N][N] = {
        {  2.0,  -1.0,   0.0,   0.0,   0.0 },
        { -1.0,   2.0,  -1.0,   0.0,   0.0 },
        {  0.0,  -1.0,   2.0,  -1.0,   0.0 },
        {  0.0,   0.0,  -1.0,   2.0,  -1.0 },
        {  0.0,   0.0,   0.0,  -1.0,   2.0 }
    }; // A tridiagonal symmetric positive definite matrix

#if 0
    double A[N][N] = {
        {2.0, -1.0, -2.0},
        {-4.0, 6.0, 3.0},
        {-4.0, -2.0, 8.0}
    };
#endif
    double L[N][N] = {0};
    double U[N][N] = {0};

    lu_decomposition(A, L, U);

    print_matrix("A", A);
    print_matrix("L", L);
    print_matrix("U", U);

    return 0;
}

