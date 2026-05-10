#include "libmin.h"

// VIP_ENCCHAR => int8_t
// VIP_ENCBOOL => uint64_t
// VIP_ENCUINT64 => uint64_t
// VIP_ENCFLOAT => fp32_t
// VIP_ENCDOUBLE => fp64_t

// supported sizes: 256, 512, 1024 (default), 2048
#ifndef DATASET_SIZE
#define DATASET_SIZE 256
#endif

static int64_t raw_data[DATASET_SIZE];

#define C 1

static void
print_data(const int64_t *data, uint64_t size)
{
  libmin_printf("DATA DUMP:\n");
  for (uint64_t i = 0; i < size; i++)
    libmin_printf("  data[%lu] = %ld\n", i, data[i]);
}

static inline void
exchange(int *a, int i, int j, uint64_t *swaps)
{
  int temp = a[i];
  a[i] = a[j];
  a[j] = temp;
  *swaps = *swaps + 1;
}

static inline void
compareExchange(int64_t *a, int i, int j, uint64_t *swaps)
{
  int64_t pred = (((int64_t)(i < j) && (a[i] > a[j]))
                   || ((int64_t)(i > j) && (a[i] < a[j])));
  int64_t temp = a[i];
  a[i] = pred ? a[j] : a[i];
  a[j] = pred ? temp : a[j];
  *swaps = *swaps + 1;
}

static void
permuteRandom(int *a, int size, uint64_t *swaps)
{
  for (int i = 0; i < size; i++)
    exchange(a, i, (int)(libmin_rand() % ((size - i) + i)), swaps);
}

static void
compareRegions(int64_t *a, int s, int t, int offset, uint64_t *swaps)
{
  int mate[offset];
  for (int count = 0; count < C; count++)
  {
    for (int i = 0; i < offset; i++)
      mate[i] = i;
    permuteRandom(mate, offset, swaps);
    for (int i = 0; i < offset; i++)
      compareExchange(a, s + i, t + mate[i], swaps);
  }
}

static void
randshellsort(int64_t *data, uint64_t size, uint64_t *swaps)
{
  int n = (int)size;

  for (int offset = n / 2; offset > 0; offset /= 2)
  {
    for (int i = 0; i < n - offset; i += offset)
      compareRegions(data, i, i + offset, offset, swaps);
    for (int i = n - offset; i >= offset; i -= offset)
      compareRegions(data, i - offset, i, offset, swaps);
    for (int i = 0; i < n - 3 * offset; i += offset)
      compareRegions(data, i, i + 3 * offset, offset, swaps);
    for (int i = 0; i < n - 2 * offset; i += offset)
      compareRegions(data, i, i + 2 * offset, offset, swaps);
    for (int i = 0; i < n; i += 2 * offset)
      compareRegions(data, i, i + offset, offset, swaps);
    for (int i = offset; i < n - offset; i += 2 * offset)
      compareRegions(data, i, i + offset, offset, swaps);
  }
}

int
main(void)
{
  // initialize the pseudo-RNG
  libmin_srand(42);

  // encrypted values are initialized in main
  int64_t secret_data[DATASET_SIZE];
  uint64_t swaps = 0;

  // initialize the array to sort
  for (uint64_t i = 0; i < DATASET_SIZE; i++)
  {
    raw_data[i] = (int64_t)libmin_rand();
    secret_data[i] = raw_data[i];
  }
  print_data(raw_data, DATASET_SIZE);

  randshellsort(secret_data, DATASET_SIZE, &swaps);

  for (uint64_t i = 0; i < DATASET_SIZE; i++)
    raw_data[i] = secret_data[i];

  print_data(raw_data, DATASET_SIZE);

  for (uint64_t i = 0; i < DATASET_SIZE - 1; i++)
  {
    if (raw_data[i] > raw_data[i + 1])
    {
      libmin_printf("ERROR: data is not properly sorted.\n");
      return -1;
    }
  }

  libmin_printf("INFO: %lu swaps executed.\n", swaps);
  libmin_printf("INFO: data is properly sorted.\n");

  libmin_success();
  return 0;
}
