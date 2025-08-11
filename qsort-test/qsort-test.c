#include "libmin.h"

/* Comparison function for integers. */
int int_compare(const void *a, const void *b) {
    const int *ia = (const int *) a;
    const int *ib = (const int *) b;
    return (*ia) - (*ib);
}

/* Utility function to check if an array of integers is sorted (non-decreasing order). */
int is_sorted(const int arr[], size_t n) {
    for (size_t i = 1; i < n; i++) {
        if (arr[i - 1] > arr[i])
            return 0;
    }
    return 1;
}

/* Function to print an array of integers. */
void print_array(const int arr[], size_t n) {
    for (size_t i = 0; i < n; i++)
        libmin_printf("%d ", arr[i]);
    libmin_printf("\n");
}

/* Comparison function for strings using strcmp.
 * Note: Each element is a pointer to char.
 */
int string_compare(const void *a, const void *b) {
    const char * const *str1 = (const char * const *) a;
    const char * const *str2 = (const char * const *) b;
    return libmin_strcmp(*str1, *str2);
}

/* Utility function to check if an array of strings is sorted lexicographically. */
int is_sorted_string_array(const char *arr[], size_t n) {
    for (size_t i = 1; i < n; i++) {
        if (libmin_strcmp(arr[i - 1], arr[i]) > 0)
            return 0;
    }
    return 1;
}

/* Function to print an array of strings. */
void print_string_array(const char *arr[], size_t n) {
    for (size_t i = 0; i < n; i++) {
        libmin_printf("%s ", arr[i]);
    }
    libmin_printf("\n");
}

int main(void) {
    /* ---------------------------
     * Test 1: Random integer array (40 elements).
     * These values are mostly unique with only few duplicates.
     * ---------------------------
     */
    int test1[40] = {
         5,  3, 12,  8,  1, 19,  7,  4,  0,  6,
        10, 15,  2, 22, 11, 17, 13, 20,  9, 14,
        16, 21, 23, 18, 24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35, 36, 37, 38, 39
    };
    size_t n1 = sizeof(test1) / sizeof(test1[0]);
    libtarg_start_perf();
    libmin_qsort(test1, n1, sizeof(int), int_compare);

    /* ---------------------------
     * Test 2: Integer array with few duplicates (40 elements).
     * Only one duplicate (the value '5' appears twice).
     * ---------------------------
     */
    int test2[40] = {
         2,  1,  3,  0,  4,  5,  5,  7,  8,  9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35, 36, 37, 38, 39
    };
    size_t n2 = sizeof(test2) / sizeof(test2[0]);
    libmin_qsort(test2, n2, sizeof(int), int_compare);

    /* ---------------------------
     * Test 3: Already sorted integer array (40 elements).
     * Generate 40 ascending integers (0, 1, 2, ... 39).
     * ---------------------------
     */
    int test3[40];
    for (int i = 0; i < 40; i++) {
        test3[i] = i;
    }
    libmin_qsort(test3, 40, sizeof(int), int_compare);

    /* ---------------------------
     * Test 4: Reverse sorted integer array (40 elements).
     * Generate 40 descending integers (39, 38, ... 0).
     * ---------------------------
     */
    int test4[40];
    for (int i = 0; i < 40; i++) {
        test4[i] = 39 - i;
    }
    libmin_qsort(test4, 40, sizeof(int), int_compare);

    /* ---------------------------
     * Test 5: String array (32 elements).
     * These 32 strings are mostly distinct with one duplicate ("apple").
     * ---------------------------
     */
    const char *test_strings[32] = {
        "banana", "apple", "orange", "kiwi", "grape", "mango", "strawberry", "pear",
        "pineapple", "watermelon", "blueberry", "raspberry", "blackberry", "cherry", "peach", "apricot",
        "plum", "nectarine", "cantaloupe", "honeydew", "papaya", "passionfruit", "guava", "lychee",
        "pomegranate", "grapefruit", "lemon", "lime", "tangerine", "cranberry", "fig", "apple"  // "apple" appears twice
    };
    size_t n_str = sizeof(test_strings) / sizeof(test_strings[0]);
    libmin_qsort(test_strings, n_str, sizeof(char *), string_compare);
    libtarg_stop_perf();

    libmin_printf("Test 1: Sorted Random Integer Array (40 elements):\n");
    print_array(test1, n1);
    if (is_sorted(test1, n1))
        libmin_printf("Test 1 passed: array sorted correctly.\n");
    else
        libmin_printf("Test 1 failed: array not sorted correctly.\n");

    libmin_printf("\nTest 2: Sorted Integer Array with Few Duplicates (40 elements):\n");
    print_array(test2, n2);
    if (is_sorted(test2, n2))
        libmin_printf("Test 2 passed: duplicate elements sorted correctly.\n");
    else
        libmin_printf("Test 2 failed: duplicate elements not sorted correctly.\n");

    libmin_printf("\nTest 3: Already Sorted Integer Array (40 elements):\n");
    print_array(test3, 40);
    if (is_sorted(test3, 40))
        libmin_printf("Test 3 passed: array remains sorted.\n");
    else
        libmin_printf("Test 3 failed: already sorted array not sorted correctly.\n");

    libmin_printf("\nTest 4: Sorted Reverse Order Integer Array (40 elements):\n");
    print_array(test4, 40);
    if (is_sorted(test4, 40))
        libmin_printf("Test 4 passed: reverse sorted array sorted correctly.\n");
    else
        libmin_printf("Test 4 failed: reverse sorted array not sorted correctly.\n");

    libmin_printf("\nTest 5: Sorted String Array (32 elements):\n");
    print_string_array(test_strings, n_str);
    if (is_sorted_string_array(test_strings, n_str))
        libmin_printf("Test 5 passed: string array sorted correctly.\n");
    else
        libmin_printf("Test 5 failed: string array not sorted correctly.\n");

    libmin_success();
    return 0;
}

