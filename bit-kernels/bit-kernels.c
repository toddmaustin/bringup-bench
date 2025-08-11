#include "libmin.h"

#define NUM_ELEMENTS 100   // Total numbers to test
#define TABLE_SIZE 256     // For table lookup; precompute for 8-bit numbers

// Global lookup table for table method
unsigned int popcount_table[TABLE_SIZE];

// ------------------------------
// Bit-counting Kernel 1: Naive method
// ------------------------------
unsigned int count_bits_naive(uint32_t x) {
    unsigned int count = 0;
    while (x) {
        count += (x & 1);
        x >>= 1;
    }
    return count;
}

// ------------------------------
// Bit-counting Kernel 2: Kernighan's algorithm
// ------------------------------
unsigned int count_bits_kernighan(uint32_t x) {
    unsigned int count = 0;
    while (x) {
        x &= (x - 1);
        count++;
    }
    return count;
}

// ------------------------------
// Bit-counting Kernel 3: Compiler built-in popcount (GCC/Clang)
// ------------------------------
unsigned int count_bits_builtin(uint32_t x) {
    return __builtin_popcount(x);
}

// ------------------------------
// Bit-counting Kernel 4: Table lookup
// ------------------------------
// Initialize the lookup table for 8-bit popcount.
void init_popcount_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        unsigned int count = 0;
        int value = i;
        while (value) {
            count += (value & 1);
            value >>= 1;
        }
        popcount_table[i] = count;
    }
}
  
// Use the table to compute popcount for a 32-bit integer.
unsigned int count_bits_table(uint32_t x) {
    return popcount_table[x & 0xFF] +
           popcount_table[(x >> 8) & 0xFF] +
           popcount_table[(x >> 16) & 0xFF] +
           popcount_table[(x >> 24) & 0xFF];
}

// ------------------------------
// Bit-counting Kernel 5: Parallel counting (SWAR algorithm)
// ------------------------------
unsigned int count_bits_parallel(uint32_t x) {
    // Subtract pairs of bits
    x = x - ((x >> 1) & 0x55555555);
    // Sum bits in nibble groups
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    // Aggregate counts in bytes
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    // Multiply and shift out the sum
    return (x * 0x01010101) >> 24;
}

// ------------------------------
// Main testing function
// ------------------------------
int main(void) {
    // Seed random generator
    libmin_srand(42);
    
    // Initialize lookup table for table lookup method.
    init_popcount_table();
    
    // Allocate an array of NUM_ELEMENTS random 32-bit numbers.
    uint32_t *numbers = libmin_malloc(NUM_ELEMENTS * sizeof(uint32_t));
    if (numbers == NULL) {
        libmin_printf("ERROR: Memory allocation error!\n");
        return 1;
    }
    
    // Fill the array with random 32-bit numbers.
    for (size_t i = 0; i < NUM_ELEMENTS; i++) {
        // Combine two calls to rand() to create a 32-bit number.
        numbers[i] = ((uint32_t)libmin_rand() << 16) | (libmin_rand() & 0xFFFF);
    }
    
    // Variables to sum total bit counts for each method.
    unsigned long long total_naive = 0;
    unsigned long long total_kernighan = 0;
    unsigned long long total_builtin = 0;
    unsigned long long total_table = 0;
    unsigned long long total_parallel = 0;
    
    // Process each number.
    libtarg_start_perf();
    for (size_t i = 0; i < NUM_ELEMENTS; i++) {
        uint32_t val = numbers[i];
        unsigned int naive   = count_bits_naive(val);
        unsigned int kernighan = count_bits_kernighan(val);
        unsigned int builtin = count_bits_builtin(val);
        unsigned int table   = count_bits_table(val);
        unsigned int parallel = count_bits_parallel(val);
        
        // Verify that all methods agree.
        if (naive != kernighan || naive != builtin || naive != table || naive != parallel) {
            libmin_printf("Mismatch for value 0x%08X: naive=%u, kernighan=%u, builtin=%u, table=%u, parallel=%u\n",
                   val, naive, kernighan, builtin, table, parallel);
        }
        
        // Accumulate totals.
        total_naive   += naive;
        total_kernighan += kernighan;
        total_builtin += builtin;
        total_table   += table;
        total_parallel += parallel;
        
        // For demonstration, print the first 5 numbers with their popcount from each method.
        if (i < 5) {
            libmin_printf("Value 0x%08X: naive=%2u, kernighan=%2u, builtin=%2u, table=%2u, parallel=%2u\n",
                   val, naive, kernighan, builtin, table, parallel);
        }
    }
    libtarg_stop_perf();
    
    // Print overall totals for comparison.
    libmin_printf("\nTotal bit count over %d numbers:\n", NUM_ELEMENTS);
    libmin_printf("Naive         : %llu\n", total_naive);
    libmin_printf("Kernighan     : %llu\n", total_kernighan);
    libmin_printf("Built-in      : %llu\n", total_builtin);
    libmin_printf("Table Lookup  : %llu\n", total_table);
    libmin_printf("Parallel      : %llu\n", total_parallel);
    
    // Clean up
    libmin_free(numbers);
    libmin_success();
    return 0;
}

