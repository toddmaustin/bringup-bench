#include "libmin.h"

#define N 8
#define PI 3.14159265358979323846

double C(int u) {
    return (u == 0) ? libmin_sqrt(1.0 / N) : libmin_sqrt(2.0 / N);
}

void idct_2d(double input[N][N], double output[N][N]) {
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            double sum = 0.0;
            for (int u = 0; u < N; u++) {
                for (int v = 0; v < N; v++) {
                    sum += C(u) * C(v) * input[u][v] *
                        libmin_cos(((2 * x + 1) * u * PI) / (2 * N)) *
                        libmin_cos(((2 * y + 1) * v * PI) / (2 * N));
                }
            }
            output[x][y] = sum;
        }
    }
}

void print_matrix(double mat[N][N], const char* label) {
    libmin_printf("%s:\n", label);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            libmin_printf("%8.2f ", mat[i][j]);
        libmin_printf("\n");
    }
    libmin_printf("\n");
}

int main() {
    // Example: DCT domain input (can be real JPEG coefficients)
    double input[N][N] = {
        {52, 0, 0, 0, 0, 0, 0, 0},
        {30, 0, 0, 0, 0, 0, 0, 0},
        {20, 0, 0, 0, 0, 0, 0, 0},
        {10, 0, 0, 0, 0, 0, 0, 0},
        { 5, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0},
    };

    double output[N][N] = {0};

    idct_2d(input, output);
    print_matrix(output, "IDCT Output");

    libmin_success();
    return 0;
}
