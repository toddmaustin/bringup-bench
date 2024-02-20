// The longest common subsequence in C

#include "libmin.h"

char S1[20] = "ACADBFEDSFSDFDSEFE", S2[20] = "CBDAEFEFESAFEASDD";

int i, j, m, n, LCS_table[40][40];
char b[40][40];

void lcsAlgo() {
  // Filling 0's in the matrix
  for (i = 0; i <= m; i++)
    LCS_table[i][0] = 0;
  for (i = 0; i <= n; i++)
    LCS_table[0][i] = 0;

  // Building the mtrix in bottom-up way
  for (i = 1; i <= m; i++)
    for (j = 1; j <= n; j++) {
      if (S1[i - 1] == S2[j - 1]) {
        LCS_table[i][j] = LCS_table[i - 1][j - 1] + 1;
      } else if (LCS_table[i - 1][j] >= LCS_table[i][j - 1]) {
        LCS_table[i][j] = LCS_table[i - 1][j];
      } else {
        LCS_table[i][j] = LCS_table[i][j - 1];
      }
    }

  int index = LCS_table[m][n];
  char lcsAlgo[index + 1];
  lcsAlgo[index] = '\0';

  int i = m, j = n;
  while (i > 0 && j > 0) {
    if (S1[i - 1] == S2[j - 1]) {
      lcsAlgo[index - 1] = S1[i - 1];
      i--;
      j--;
      index--;
    }

    else if (LCS_table[i - 1][j] > LCS_table[i][j - 1])
      i--;
    else
      j--;
  }

  // Printing the sub sequences
  libmin_printf("LCS: %s", lcsAlgo);
}

int
main(void)
{
  m = libmin_strlen(S1);
  n = libmin_strlen(S2);

  libmin_printf("S1 : %s (%d) \nS2 : %s  (%d)\n", S1, m, S2, n);
  lcsAlgo();
  libmin_printf("\n");

  libmin_success();
  return 0;
}
