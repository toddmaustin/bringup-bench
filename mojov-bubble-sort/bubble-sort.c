#include "libmin.h"

#if 0
#define SECRET __attribute__((__secret__))
#else
#define SECRET
#endif
// #define secret_cmov(p, x, y) __builtin_secret_cmov((p), (x), (y))
#define secret_cmov(p, x, y) ((p) ? (x) : (y))
#define secret_decrypt(x) (x)

extern inline uint64_t
__instret(void)
{
  uint64_t insts;
  __asm__ volatile ("rdinstret %0" : "=r"(insts));

  return insts;
}

#if 0
SECRET int
secret_cmov(SECRET bool p, SECRET int x, SECRET int y)
{
  return (int)p*x + (int)!p*y;
}
#endif


// supported sizes: 256 (default), 512, 1024, 2048
#define DATASET_SIZE 256
int raw_data[DATASET_SIZE];
SECRET int secret_data[DATASET_SIZE];

// total swaps executed so far
SECRET unsigned long swaps = 0;


void
print_data(int *data, unsigned size)
{
  libmin_printf("DATA DUMP:\n");
  for (unsigned i=0; i < size; i++)
    libmin_printf("  data[%u] = %d\n", i, data[i]);
}

void
bubblesort(SECRET int *data, unsigned size)
{
  for (unsigned i=0; i < size-1; i++)
  {
    for (unsigned j=0; j < size-1; j++)
    {
      // swap needed?
      SECRET bool swap = (data[j] > data[j+1]);

      // perform the swap
      SECRET int tmp = data[j];
      data[j] = secret_cmov(swap, data[j+1], data[j]);
      data[j+1] = secret_cmov(swap, tmp, data[j+1]);

      // count the number of swaps executed
      swaps = secret_cmov(swap, swaps+1, swaps);
    }
  }
}

int
main(void)
{
  // initialize the pseudo-RNG
  libmin_srand(42);

  // initialize the array to sort
  for (unsigned i=0; i < DATASET_SIZE; i++)
    secret_data[i] = raw_data[i] = libmin_rand();
  print_data(raw_data, DATASET_SIZE);

  {
    // performance monitoring
    uint64_t icnt_start = __instret();

    bubblesort(secret_data, DATASET_SIZE);

    uint64_t icnt_end = __instret();
    libmin_printf("INFO: bubblesort inst count = %lu.\n", icnt_end - icnt_start + 1);
  }


  // decrypt the array
  for (unsigned i=0; i < DATASET_SIZE; i++)
    raw_data[i] = secret_decrypt(secret_data[i]);
  print_data(raw_data, DATASET_SIZE);

  // check the array
  for (unsigned i=0; i < DATASET_SIZE-1; i++)
  {
    if (raw_data[i] > raw_data[i+1])
    {
      libmin_printf("ERROR: data is not properly sorted.\n");
      return -1;
    }
  }
  libmin_printf("INFO: %lu swaps executed.\n", secret_decrypt(swaps));
  libmin_printf("INFO: data is properly sorted.\n");

  libmin_success();
  return 0;
}
