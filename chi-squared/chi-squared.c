#include "libmin.h"

#define DATASET_SIZE 600
#define ASSOC_X_CATEGORIES 4
#define ASSOC_Y_CATEGORIES 3
#define GOF_CATEGORIES 5

static const uint64_t gof_expected_counts[GOF_CATEGORIES] = {60, 120, 180, 150, 90};

static inline uint64_t
my_abs_diff(uint64_t lhs, uint64_t rhs)
{
  if (lhs >= rhs)
    return lhs - rhs;
  else
    return rhs - lhs;
}

static inline uint64_t
plaintext_abs_diff(uint64_t lhs, uint64_t rhs)
{
  return (lhs >= rhs) ? (lhs - rhs) : (rhs - lhs);
}

int
main(void)
{
  uint64_t assoc_x[DATASET_SIZE];
  uint64_t assoc_y[DATASET_SIZE];
  uint64_t gof_values[DATASET_SIZE];

  for (uint64_t i = 0; i < DATASET_SIZE; ++i)
  {
    uint64_t x = ((i * 17) + (i / 7) + 3) % ASSOC_X_CATEGORIES;
    uint64_t y = ((i * 13) + (i / 11) + 1) % ASSOC_Y_CATEGORIES;
    uint64_t g = ((i * 19) + (i / 5) + x + (2 * y)) % GOF_CATEGORIES;

    assoc_x[i] = x;
    assoc_y[i] = y;
    gof_values[i] = g;
  }

  uint64_t contingency[ASSOC_X_CATEGORIES][ASSOC_Y_CATEGORIES] = {0};
  uint64_t row_totals[ASSOC_X_CATEGORIES] = {0};
  uint64_t col_totals[ASSOC_Y_CATEGORIES] = {0};
  uint64_t gof_observed[GOF_CATEGORIES] = {0};

  for (uint64_t i = 0; i < DATASET_SIZE; ++i)
  {
    for (uint64_t xr = 0; xr < ASSOC_X_CATEGORIES; ++xr)
    {
      uint64_t x_match = (assoc_x[i] == xr);
      row_totals[xr] += x_match;
      for (uint64_t yc = 0; yc < ASSOC_Y_CATEGORIES; ++yc)
      {
        uint64_t y_match = (assoc_y[i] == yc);
        contingency[xr][yc] += (x_match & y_match);
      }
    }

    for (uint64_t yc = 0; yc < ASSOC_Y_CATEGORIES; ++yc)
      col_totals[yc] += (assoc_y[i] == yc);

    for (uint64_t g = 0; g < GOF_CATEGORIES; ++g)
      gof_observed[g] += (gof_values[i] == g);
  }

  uint64_t association_chi_scaled = 0;
  for (uint64_t xr = 0; xr < ASSOC_X_CATEGORIES; ++xr)
  {
    for (uint64_t yc = 0; yc < ASSOC_Y_CATEGORIES; ++yc)
    {
      uint64_t observed_times_n = contingency[xr][yc] * DATASET_SIZE;
      uint64_t row_col_product = row_totals[xr] * col_totals[yc];
      uint64_t abs_delta = my_abs_diff(observed_times_n, row_col_product);
      uint64_t numerator = abs_delta * abs_delta * 1000;
      uint64_t denominator = (row_col_product * DATASET_SIZE) + (row_col_product == 0);
      association_chi_scaled += numerator / denominator;
    }
  }

  uint64_t gof_chi_scaled = 0;
  for (uint64_t g = 0; g < GOF_CATEGORIES; ++g)
  {
    uint64_t expected = gof_expected_counts[g];
    uint64_t abs_delta = my_abs_diff(gof_observed[g], expected);
    gof_chi_scaled += (abs_delta * abs_delta * 1000) / expected;
  }

  uint64_t assoc_reject_null = (association_chi_scaled >= 12592);
  uint64_t gof_reject_null = (gof_chi_scaled >= 9488);

  uint64_t contingency_plain[ASSOC_X_CATEGORIES][ASSOC_Y_CATEGORIES];
  uint64_t row_totals_plain[ASSOC_X_CATEGORIES];
  uint64_t col_totals_plain[ASSOC_Y_CATEGORIES];
  uint64_t gof_observed_plain[GOF_CATEGORIES];

  for (uint64_t xr = 0; xr < ASSOC_X_CATEGORIES; ++xr)
  {
    row_totals_plain[xr] = row_totals[xr];
    for (uint64_t yc = 0; yc < ASSOC_Y_CATEGORIES; ++yc)
      contingency_plain[xr][yc] = contingency[xr][yc];
  }

  for (uint64_t yc = 0; yc < ASSOC_Y_CATEGORIES; ++yc)
    col_totals_plain[yc] = col_totals[yc];

  for (uint64_t g = 0; g < GOF_CATEGORIES; ++g)
    gof_observed_plain[g] = gof_observed[g];

  uint64_t association_chi_plain = association_chi_scaled;
  uint64_t gof_chi_plain = gof_chi_scaled;
  uint64_t assoc_reject_plain = assoc_reject_null;
  uint64_t gof_reject_plain = gof_reject_null;

  uint64_t expected_contingency[ASSOC_X_CATEGORIES][ASSOC_Y_CATEGORIES] = {{0}};
  uint64_t expected_row_totals[ASSOC_X_CATEGORIES] = {0};
  uint64_t expected_col_totals[ASSOC_Y_CATEGORIES] = {0};
  uint64_t expected_gof_observed[GOF_CATEGORIES] = {0};

  for (uint64_t i = 0; i < DATASET_SIZE; ++i)
  {
    uint64_t x = ((i * 17) + (i / 7) + 3) % ASSOC_X_CATEGORIES;
    uint64_t y = ((i * 13) + (i / 11) + 1) % ASSOC_Y_CATEGORIES;
    uint64_t g = ((i * 19) + (i / 5) + x + (2 * y)) % GOF_CATEGORIES;

    expected_contingency[x][y] += 1;
    expected_row_totals[x] += 1;
    expected_col_totals[y] += 1;
    expected_gof_observed[g] += 1;
  }

  uint64_t expected_association_chi = 0;
  for (uint64_t xr = 0; xr < ASSOC_X_CATEGORIES; ++xr)
  {
    for (uint64_t yc = 0; yc < ASSOC_Y_CATEGORIES; ++yc)
    {
      uint64_t observed_times_n = expected_contingency[xr][yc] * DATASET_SIZE;
      uint64_t row_col_product = expected_row_totals[xr] * expected_col_totals[yc];
      uint64_t abs_delta = plaintext_abs_diff(observed_times_n, row_col_product);
      expected_association_chi += (abs_delta * abs_delta * 1000) / (row_col_product * DATASET_SIZE);
    }
  }

  uint64_t expected_gof_chi = 0;
  for (uint64_t g = 0; g < GOF_CATEGORIES; ++g)
  {
    uint64_t abs_delta = plaintext_abs_diff(expected_gof_observed[g], gof_expected_counts[g]);
    expected_gof_chi += (abs_delta * abs_delta * 1000) / gof_expected_counts[g];
  }

  libmin_printf("chi-squared dataset-size=%u association-categories=%ux%u gof-categories=%u\n",
    DATASET_SIZE, ASSOC_X_CATEGORIES, ASSOC_Y_CATEGORIES, GOF_CATEGORIES);

  libmin_printf("chi-squared association contingency:\n");
  for (uint64_t xr = 0; xr < ASSOC_X_CATEGORIES; ++xr)
  {
    libmin_printf("  row%u: %lu %lu %lu (row-total=%lu)\n",
      xr,
      contingency_plain[xr][0],
      contingency_plain[xr][1],
      contingency_plain[xr][2],
      row_totals_plain[xr]);
  }

  libmin_printf("  col-totals: %lu %lu %lu\n",
    col_totals_plain[0],
    col_totals_plain[1],
    col_totals_plain[2]);

  libmin_printf("chi-squared goodness-of-fit observed: %lu %lu %lu %lu %lu\n",
    gof_observed_plain[0],
    gof_observed_plain[1],
    gof_observed_plain[2],
    gof_observed_plain[3],
    gof_observed_plain[4]);

  libmin_printf("chi-squared goodness-of-fit expected: %lu %lu %lu %lu %lu\n",
    gof_expected_counts[0],
    gof_expected_counts[1],
    gof_expected_counts[2],
    gof_expected_counts[3],
    gof_expected_counts[4]);

  libmin_printf("chi-squared association-statistic-scaled-x1000=%lu reject-null(alpha=0.05)=%s\n",
    association_chi_plain,
    assoc_reject_plain ? "yes" : "no");

  libmin_printf("chi-squared goodness-of-fit-statistic-scaled-x1000=%lu reject-null(alpha=0.05)=%s\n",
    gof_chi_plain,
    gof_reject_plain ? "yes" : "no");

  if (association_chi_plain != expected_association_chi || gof_chi_plain != expected_gof_chi)
  {
    libmin_printf("ERROR: chi-squared statistics mismatch\n");
    return -1;
  }

  for (uint64_t xr = 0; xr < ASSOC_X_CATEGORIES; ++xr)
  {
    if (row_totals_plain[xr] != expected_row_totals[xr])
      return -1;
    for (uint64_t yc = 0; yc < ASSOC_Y_CATEGORIES; ++yc)
      if (contingency_plain[xr][yc] != expected_contingency[xr][yc])
        return -1;
  }

  for (uint64_t yc = 0; yc < ASSOC_Y_CATEGORIES; ++yc)
    if (col_totals_plain[yc] != expected_col_totals[yc])
      return -1;

  for (uint64_t g = 0; g < GOF_CATEGORIES; ++g)
    if (gof_observed_plain[g] != expected_gof_observed[g])
      return -1;

  libmin_printf("chi-squared: categorical analysis completed for %u records\n", DATASET_SIZE);

  libmin_success();
  return 0;
}
