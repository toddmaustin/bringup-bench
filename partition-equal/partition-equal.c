#include "libmin.h"

#define N_SETS 8
#define MAX_ELEMS 8
#define MAX_SUM 40

typedef struct {
  uint64_t len;
  uint64_t values[MAX_ELEMS];
} partition_set_t;

typedef struct {
  uint64_t can_partition;
  uint64_t subset_mask;
} partition_result_t;

static const partition_set_t partition_sets[N_SETS] = {
  {4, {1, 5, 11, 5, 0, 0, 0, 0}},
  {4, {1, 2, 3, 5, 0, 0, 0, 0}},
  {5, {3, 1, 1, 2, 2, 0, 0, 0}},
  {4, {2, 2, 3, 5, 0, 0, 0, 0}},
  {6, {4, 4, 4, 4, 4, 4, 0, 0}},
  {6, {2, 9, 10, 1, 1, 1, 0, 0}},
  {8, {7, 3, 2, 5, 8, 1, 6, 4}},
  {7, {10, 10, 10, 10, 5, 5, 5, 0}}
};

static uint64_t
plain_partition_equal(const partition_set_t *set)
{
  uint64_t sum = 0;
  for (uint64_t i = 0; i < set->len; ++i)
    sum += set->values[i];

  if ((sum & 1ul) != 0)
    return 0;

  uint64_t target = sum / 2;
  uint8_t dp[MAX_SUM + 1];
  for (uint64_t s = 0; s <= MAX_SUM; ++s)
    dp[s] = 0;
  dp[0] = 1;

  for (uint64_t i = 0; i < set->len; ++i)
  {
    uint64_t val = set->values[i];
    for (int64_t s = (int64_t)target; s >= (int64_t)val; --s)
      dp[s] = dp[s] || dp[s - val];
  }

  return dp[target];
}

static partition_result_t
encrypted_partition_equal(const partition_set_t *set)
{
  uint64_t sum = 0;
  for (uint64_t i = 0; i < set->len; ++i)
    sum = sum + (uint64_t)(set->values[i]);

  uint64_t target = sum / 2;
  uint64_t odd_sum = sum & 1;

  uint64_t reachable[MAX_ELEMS + 1][MAX_SUM + 1];
  for (uint64_t i = 0; i <= set->len; ++i)
  {
    for (uint64_t s = 0; s <= MAX_SUM; ++s)
      reachable[i][s] = 0;
  }
  reachable[0][0] = 1;

  for (uint64_t i = 0; i < set->len; ++i)
  {
    uint64_t value = (uint64_t)(set->values[i]);

    for (uint64_t s = 0; s <= MAX_SUM; ++s)
    {
      uint64_t not_take = reachable[i][s];
      uint64_t take = 0;

      for (uint64_t prev = 0; prev <= MAX_SUM; ++prev)
      {
        uint64_t transition = ((uint64_t)(prev) + value) == (uint64_t)(s);
        take = take || (reachable[i][prev] && transition);
      }

      reachable[i + 1][s] = not_take || take;
    }
  }

  uint64_t reachable_target = 0;
  for (uint64_t s = 0; s <= MAX_SUM; ++s)
  {
    uint64_t is_target = (uint64_t)(s) == target;
    reachable_target = reachable_target || (reachable[set->len][s] && is_target);
  }

  uint64_t can_partition = (odd_sum == 0) && reachable_target;

  uint64_t current_sum = target;
  uint64_t subset_mask = 0;

  for (int64_t i = (int64_t)set->len - 1; i >= 0; --i)
  {
    uint64_t value = (uint64_t)(set->values[i]);
    uint64_t found = 0;
    uint64_t prev_sum_choice = current_sum;

    for (uint64_t prev = 0; prev <= MAX_SUM; ++prev)
    {
      uint64_t matches = reachable[i][prev] && (((uint64_t)(prev) + value) == current_sum);
      uint64_t choose_here = (!found) && matches;
      prev_sum_choice = choose_here ? (uint64_t)(prev) : prev_sum_choice;
      found = found || matches;
    }

    uint64_t take = can_partition && found;
    subset_mask = subset_mask | (take ? (uint64_t)(1ul << i) : (uint64_t)(0));
    current_sum = take ? prev_sum_choice : current_sum;
  }

  partition_result_t ret;
  ret.can_partition = can_partition;
  ret.subset_mask = subset_mask;
  return ret;
}

static void
print_partition_from_mask(const partition_set_t *set, uint64_t mask)
{
  uint64_t left_sum = 0;
  uint64_t right_sum = 0;

  libmin_printf("  partition A: {");
  uint64_t first = 1;
  for (uint64_t i = 0; i < set->len; ++i)
  {
    if ((mask & (1ul << i)) != 0)
    {
      if (!first)
        libmin_printf(", ");
      libmin_printf("%lu", set->values[i]);
      first = 0;
      left_sum += set->values[i];
    }
    else
      right_sum += set->values[i];
  }
  libmin_printf("} (sum=%lu)\n", left_sum);

  libmin_printf("  partition B: {");
  first = 1;
  for (uint64_t i = 0; i < set->len; ++i)
  {
    if ((mask & (1ul << i)) == 0)
    {
      if (!first)
        libmin_printf(", ");
      libmin_printf("%lu", set->values[i]);
      first = 0;
    }
  }
  libmin_printf("} (sum=%lu)\n", right_sum);
}

int
main(void)
{
  partition_result_t secret_results[N_SETS];

  libmin_printf("partition-equal: processing %u encrypted sets\n", N_SETS);

  for (uint64_t i = 0; i < N_SETS; ++i)
    secret_results[i] = encrypted_partition_equal(&partition_sets[i]);

  uint64_t pass_count = 0;
  for (uint64_t i = 0; i < N_SETS; ++i)
  {
    uint64_t revealed = secret_results[i].can_partition;
    uint64_t revealed_mask = secret_results[i].subset_mask;
    uint64_t expected = plain_partition_equal(&partition_sets[i]);

    libmin_printf("partition-equal set=%lu can_partition=%s\n",
      i, revealed ? "YES" : "NO");

    if (revealed)
      print_partition_from_mask(&partition_sets[i], revealed_mask);

    if (revealed != expected)
    {
      libmin_printf("partition-equal ERROR: mismatch at set %lu\n", i);
      return -1;
    }

    pass_count += 1;
  }

  libmin_printf("partition-equal PASS: validated %lu encrypted sets\n", pass_count);

  libmin_success();
  return 0;
}
