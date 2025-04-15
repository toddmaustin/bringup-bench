#include "libmin.h"
#include "libtarg.h"

/*
 * swap - Exchange the contents of two elements.
 * This version allocates a temporary buffer using malloc()/free() to avoid
 * reliance on C99 variable-length arrays.
 */
static void swap(void *a, void *b, size_t size) {
    void *tmp = libmin_malloc(size);
    if (!tmp) {
        /* Allocation failed; exit or handle as appropriate. */
        libmin_fail(EXIT_FAILURE);
    }
    libmin_memcpy(tmp, a, size);
    libmin_memcpy(a, b, size);
    libmin_memcpy(b, tmp, size);
    libmin_free(tmp);
}

/*
 * qsort_internal - Recursive quicksort implementation using the Hoare partition scheme.
 * @base: The base address of the array (cast to char* for pointer arithmetic).
 * @low:  The index of the first element in the current partition.
 * @high: The index of the last element in the current partition.
 * @size: Size in bytes of each element.
 * @compar: The comparison function.
 *
 * This function partitions the sub-array [low, high] around a pivot (chosen as the
 * element at index low), then recursively sorts the resulting partitions.
 */
static void qsort_internal(char *base, size_t low, size_t high, size_t size,
                           int (*compar)(const void *, const void *)) {
    if (low >= high)
        return;

    /* Choose pivot as the first element in the partition and copy it */
    char *pivot = libmin_malloc(size);
    if (!pivot)
        libmin_fail(EXIT_FAILURE);
    libmin_memcpy(pivot, base + low * size, size);

    size_t i = low;
    size_t j = high + 1;

    /* Hoare partition scheme */
    while (1) {
        do {
            i++;
        } while (i <= high && compar(base + i * size, pivot) < 0);

        do {
            j--;
        } while (compar(base + j * size, pivot) > 0);

        if (i >= j)
            break;

        swap(base + i * size, base + j * size, size);
    }

    swap(base + low * size, base + j * size, size);
    libmin_free(pivot);

    if (j > 0)
        qsort_internal(base, low, j - 1, size, compar);
    qsort_internal(base, j + 1, high, size, compar);
}

/*
 * qsort - Public function to sort an array.
 */
void libmin_qsort(void *base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *)) {
    if (nmemb < 2)
        return;
    qsort_internal((char *)base, 0, nmemb - 1, size, compar);
}

