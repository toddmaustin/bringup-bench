/*
 * rand_test.c
 *
 * A randomness testing application that runs a battery of tests on two random
 * generators:
 *   1. A deliberately weak random generator (bad_rand).
 *   2. The system's standard library rand() function, wrapped in good_rand.
 *
 * The tests include:
 *   - Monobit test: Count the number of 1 bits in a large set of generated numbers.
 *   - Chi-square test: Check the uniformity of the top 8 bits.
 *   - Serial correlation test: Measure the correlation between successive random numbers.
 *
 * To compile:
 *      gcc rand_test.c -lm -o rand_test
 */

#include "libmin.h"

#define SAMPLE_SIZE 10000
#define BIN_COUNT 256

// Define a function pointer type for random number generators.
typedef uint32_t (*rng_func_t)(void);

// Helper: Count the number of 1 bits in a 32-bit integer.
static inline int count_bits(uint32_t x) {
    return __builtin_popcount(x);
}

// A deliberately weak ("bad") random number generator using a simple LCG.
uint32_t bad_rand(void) {
    static uint32_t state = 123456789;  // Fixed seed for reproducibility.
    state = state * 110351524 + 12345;
    return state;
}

// A wrapper for the system's rand() function.
// Since rand() may not return a full 32-bit value, we combine two calls.
uint32_t good_rand(void) {
    // Obtain two random numbers.
    uint32_t r1 = (uint32_t) libmin_rand();
    uint32_t r2 = (uint32_t) libmin_rand();
    // Combine them. This assumes rand() returns at most 15 bits.
    return (r1 << 15) | (r2 & 0x7FFF);
}

// Test harness to run the battery of randomness tests on the provided RNG.
void run_tests(const char *gen_name, rng_func_t rng_func) {
    libmin_printf("Running tests for %s random generator:\n", gen_name);

    uint64_t ones_count = 0;
    uint64_t total_bits = 0;
    uint32_t bins[BIN_COUNT] = {0};

    // Variables for the serial correlation test.
    double serial_x_sum = 0.0, serial_y_sum = 0.0;
    double serial_xy_sum = 0.0, serial_x2_sum = 0.0, serial_y2_sum = 0.0;
    int serial_count = 0;
    uint32_t prev = 0;
    int first = 1;

    // Loop to generate SAMPLE_SIZE random numbers.
    for (size_t i = 0; i < SAMPLE_SIZE; i++) {
        uint32_t sample = rng_func();

        // Monobit Test: Count the number of 1 bits.
        ones_count += count_bits(sample);
        total_bits += 32;

        // Chi-Square Test: Use the top 8 bits for binning.
        uint32_t bin = sample >> 24;
        bins[bin]++;

        // Serial Correlation Test: Compare adjacent samples.
        if (!first) {
            serial_x_sum   += (double)prev;
            serial_y_sum   += (double)sample;
            serial_xy_sum  += (double)prev * sample;
            serial_x2_sum  += (double)prev * prev;
            serial_y2_sum  += (double)sample * sample;
            serial_count++;
        } else {
            first = 0;
        }
        prev = sample;
    }

    // Report Monobit Test Results.
    double ones_ratio = (double)ones_count / total_bits;
    libmin_printf("Monobit Test:\n");
    libmin_printf("  Total bits processed: %llu\n", total_bits);
    libmin_printf("  Total ones count: %llu\n", ones_count);
    libmin_printf("  Ones ratio: %f (expected ~0.5)\n", ones_ratio);

    // Report Chi-Square Test results on top 8 bits.
    double expected_count = (double)SAMPLE_SIZE / BIN_COUNT;
    double chi_square = 0.0;
    for (int i = 0; i < BIN_COUNT; i++) {
        double diff = bins[i] - expected_count;
        chi_square += (diff * diff) / expected_count;
    }
    libmin_printf("\nChi-Square Test (uniformity of top 8 bits):\n");
    libmin_printf("  Chi-square statistic: %f (degrees of freedom = %d)\n", chi_square, BIN_COUNT - 1);

    // Report Serial Correlation Test results.
    double mean_x = serial_x_sum / serial_count;
    double mean_y = serial_y_sum / serial_count;
    double covariance = (serial_xy_sum / serial_count) - (mean_x * mean_y);
    double var_x = (serial_x2_sum / serial_count) - (mean_x * mean_x);
    double var_y = (serial_y2_sum / serial_count) - (mean_y * mean_y);
    double serial_corr = covariance / (libmin_sqrt(var_x * var_y));
    libmin_printf("\nSerial Correlation Test:\n");
    libmin_printf("  Serial correlation coefficient: %f (expected near 0)\n", serial_corr);

    libmin_printf("\n-----------------------------------------------\n\n");
}

int main(void) {
    // Seed the system's RNG for good_rand().
    libmin_srand(42);

    libtarg_start_perf();
    // First test: Use the deliberately weak generator (bad_rand).
    run_tests("Bad (bad_rand())", bad_rand);

    // Second test: Use the system's standard rand() via good_rand.
    run_tests("Good (good_rand())", good_rand);
    libtarg_stop_perf();

    libmin_success();
    return 0;
}

