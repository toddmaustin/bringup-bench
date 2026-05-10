#include "libmin.h"

#define DATASET_SIZE 128
#define WINDOW_SIZE 7

typedef struct {
  uint64_t samples[DATASET_SIZE];
} encrypted_series_t;

static uint64_t plain_samples[DATASET_SIZE];
static encrypted_series_t encrypted_samples;
static uint64_t encrypted_smoothed[DATASET_SIZE];

static void
build_plain_samples(void)
{
  for (uint64_t i = 0; i < DATASET_SIZE; ++i)
  {
    uint64_t trend = 100 + (i * 3);
    uint64_t seasonal = (i * i + 17 * i + 29) % 41;
    uint64_t burst = ((i % 19) == 0) ? 53 : 0;
    plain_samples[i] = trend + seasonal + burst;
  }
}

static void
encrypt_series(void)
{
  for (uint64_t i = 0; i < DATASET_SIZE; ++i)
    encrypted_samples.samples[i] = plain_samples[i];
}

static void
compute_encrypted_moving_average(void)
{
  uint64_t window_sum = 0;

  for (uint64_t i = 0; i < WINDOW_SIZE; ++i)
    window_sum = window_sum + encrypted_samples.samples[i];

  for (uint64_t i = 0; i < DATASET_SIZE; ++i)
  {
    uint64_t window_start = (i >= (WINDOW_SIZE - 1)) ? (i - (WINDOW_SIZE - 1)) : 0;
    uint64_t window_end = i;
    uint64_t count = window_end - window_start + 1;

    if (i >= WINDOW_SIZE)
      window_sum = window_sum - encrypted_samples.samples[i - WINDOW_SIZE];

    if (i >= WINDOW_SIZE)
      window_sum = window_sum + encrypted_samples.samples[i];

    if (i < WINDOW_SIZE)
    {
      uint64_t partial_sum = 0;
      for (uint64_t j = 0; j <= i; ++j)
        partial_sum = partial_sum + encrypted_samples.samples[j];
      encrypted_smoothed[i] = partial_sum / count;
    }
    else
      encrypted_smoothed[i] = window_sum / WINDOW_SIZE;
  }
}

static uint64_t
plain_moving_average_at(uint64_t idx)
{
  uint64_t window_start = (idx >= (WINDOW_SIZE - 1)) ? (idx - (WINDOW_SIZE - 1)) : 0;
  uint64_t sum = 0;
  uint64_t count = idx - window_start + 1;

  for (uint64_t i = window_start; i <= idx; ++i)
    sum += plain_samples[i];

  return sum / count;
}

int
main(void)
{
  build_plain_samples();
  encrypt_series();

  libmin_printf("moving-average: processing %u encrypted samples with window=%u\n",
    DATASET_SIZE, WINDOW_SIZE);

  compute_encrypted_moving_average();

  libmin_printf("moving-average smoothed dataset:\n");

  for (uint64_t i = 0; i < DATASET_SIZE; ++i)
  {
    uint64_t revealed = encrypted_smoothed[i];
    uint64_t expected = plain_moving_average_at(i);

    libmin_printf("  idx=%03lu smooth=%lu\n", i, revealed);

    if (revealed != expected)
    {
      libmin_printf("moving-average ERROR: mismatch at idx=%lu (got=%lu expected=%lu)\n",
        i, revealed, expected);
      return -1;
    }
  }

  libmin_printf("moving-average PASS: validated %u smoothed encrypted samples\n", DATASET_SIZE);

  libmin_success();
  return 0;
}
