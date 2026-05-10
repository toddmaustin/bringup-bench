#include "libmin.h"

// import test genetics data
#include "gene-data.h"

// total tests to run
#define N_TESTS 2 /* 4 */

uint64_t
min3(uint64_t x, uint64_t y, uint64_t z)
{
  if ((x < y) && (x < z))
    return x;
  else if (y < z)
    return y;
  else
    return z;
}

uint64_t
EditDistance(uint64_t *str1, uint64_t *str2)
{
  unsigned i, j;
  uint64_t edit_matrix[GENE_LEN+1][GENE_LEN+1];

  for (i = 0; i < GENE_LEN+1; i++)
    edit_matrix[i][0] = i;

  for (j = 0; j < GENE_LEN+1; j++)
    edit_matrix[0][j] = j;

  for (i = 0; i < GENE_LEN; i++ )
  {
    for (j = 0; j < GENE_LEN; j++ )
    {
      uint64_t edit;
      if (str1[i] == str2[j])
        edit = 0lu;
      else
        edit = 1lu;
      edit_matrix[i + 1][j + 1] = min3(edit_matrix[i][j+1] + 1, edit_matrix[i+1][j] + 1, edit_matrix[i][j] + edit);
    }
  }
  return edit_matrix[GENE_LEN][GENE_LEN];
}


int
main(void)
{
  // initialize the pseudo-RNG
  libmin_srand(42);

  libmin_printf("------Within the randoms array------\n");

  // encrypt test genetics data
  uint64_t gene_enc[N_GENE_DATA][GENE_LEN];
  for (unsigned i=0; i < N_GENE_DATA; i++)
  {
    for (unsigned j=0; j < GENE_LEN; j++)
      gene_enc[i][j] = gene_data[i][j];
  }

  for (unsigned i=0; i < N_TESTS; i++)
  {
    libmin_printf("++ compute distance from `%s' ++\n", gene_data[i]);
    for (unsigned j=0; j < N_GENE_DATA; j++)
    {
      uint64_t ed = EditDistance(gene_enc[i], gene_enc[j]);
      libmin_printf("  edit_distance(%s, %s) == %lu\n", gene_data[i], gene_data[j], ed);
    }
    libmin_printf("\n");
  }

  libmin_success();
  return 0;
}

