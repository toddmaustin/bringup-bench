#include "libmin.h"

#define SECRET /* _secret */
#define mojov_cmov(p, x, y) ((p) ? (x) : (y))
#define mojov_decrypt(x) (x)

// supported sizes: 256 (default), 512, 1024, 2048
#define DATASET_SIZE 256
SECRET int raw_data[DATASET_SIZE];
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
      data[j] = mojov_cmov(swap, data[j+1], data[j]);
      data[j+1] = mojov_cmov(swap, tmp, data[j+1]);

      // count the number of swaps executed
      swaps = mojov_cmov(swap, swaps+1, swaps);
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
    libtarg_start_perf();

    bubblesort(secret_data, DATASET_SIZE);

    libtarg_stop_perf();
  }


  // decrypt the array
  for (unsigned i=0; i < DATASET_SIZE; i++)
    raw_data[i] = mojov_decrypt(secret_data[i]);
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
  libmin_printf("INFO: %lu swaps executed.\n", mojov_decrypt(swaps));
  libmin_printf("INFO: data is properly sorted.\n");

  libmin_success();
  return 0;
}
