#include "libmin.h"

#define DATASET_SIZE 96
#define BOOTSTRAP_COUNT 12
#define SAMPLE_SIZE 32

typedef struct {
  uint64_t indices[SAMPLE_SIZE];
  uint64_t variability;
} bootstrap_result_t;

static const uint64_t plain_data[DATASET_SIZE] = {
  128, 131, 135, 139, 144, 149, 153, 156, 160, 163, 167, 171,
  176, 180, 183, 188, 191, 195, 200, 205, 209, 214, 219, 224,
  228, 233, 239, 244, 248, 252, 257, 263, 268, 274, 279, 285,
  291, 297, 302, 309, 315, 322, 328, 333, 339, 346, 351, 359,
  366, 373, 379, 386, 392, 398, 406, 413, 419, 427, 434, 441,
  449, 456, 464, 472, 480, 487, 495, 503, 511, 519, 528, 536,
  543, 552, 560, 569, 577, 586, 595, 604, 613, 622, 631, 640,
  649, 658, 668, 677, 687, 697, 706, 716, 727, 737, 748, 759
};

static uint64_t
plain_bootstrap_variability(const uint64_t sample_indices[SAMPLE_SIZE])
{
  uint64_t sum = 0;
  uint64_t sq_sum = 0;

  for (uint64_t i = 0; i < SAMPLE_SIZE; ++i)
  {
    uint64_t value = plain_data[sample_indices[i]];
    sum += value;
    sq_sum += value * value;
  }

  uint64_t mean = sum / SAMPLE_SIZE;
  return (sq_sum / SAMPLE_SIZE) - (mean * mean);
}

static bootstrap_result_t
encrypted_bootstrap_variability(const uint64_t encrypted_data[DATASET_SIZE])
{
  bootstrap_result_t result;
  uint64_t sum = 0;
  uint64_t sq_sum = 0;

  for (uint64_t s = 0; s < SAMPLE_SIZE; ++s)
  {
    uint64_t idx = ((uint64_t)libmin_rand()) % DATASET_SIZE;
    result.indices[s] = idx;

    uint64_t value = encrypted_data[idx];
    sum = sum + value;
    sq_sum = sq_sum + (value * value);
  }

  uint64_t mean = sum / SAMPLE_SIZE;
  result.variability = (sq_sum / SAMPLE_SIZE) - (mean * mean);
  return result;
}

int
main(void)
{
  libmin_srand(31415926);

  uint64_t encrypted_data[DATASET_SIZE];
  for (uint64_t i = 0; i < DATASET_SIZE; ++i)
    encrypted_data[i] = (uint64_t)(plain_data[i]);

  bootstrap_result_t results[BOOTSTRAP_COUNT];

  libmin_printf("variability-sample: dataset_size=%u sample_size=%u bootstrap_count=%u\n",
    DATASET_SIZE, SAMPLE_SIZE, BOOTSTRAP_COUNT);

  for (uint64_t b = 0; b < BOOTSTRAP_COUNT; ++b)
    results[b] = encrypted_bootstrap_variability(encrypted_data);

  for (uint64_t b = 0; b < BOOTSTRAP_COUNT; ++b)
  {
    uint64_t revealed = results[b].variability;
    uint64_t expected = plain_bootstrap_variability(results[b].indices);

    libmin_printf("sample=%lu variability=%lu indices=[", b, revealed);
    for (uint64_t s = 0; s < SAMPLE_SIZE; ++s)
    {
      if (s != 0)
        libmin_printf(",");
      libmin_printf("%lu", results[b].indices[s]);
    }
    libmin_printf("]\n");

    if (revealed != expected)
    {
      libmin_printf("variability-sample ERROR: mismatch in sample %lu (got=%lu expected=%lu)\n",
        b, revealed, expected);
      return -1;
    }
  }

  libmin_printf("variability-sample PASS: validated %u encrypted variability samples\n",
    BOOTSTRAP_COUNT);

  libmin_success();
  return 0;
}
