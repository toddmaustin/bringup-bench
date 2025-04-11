#include "libmin.h"

#define BOARD_SIZE 10  // You can change this to any board size (e.g., 4, 8, 12)

int solution_count = 0;

// Check if placing a queen at (row, col) is safe
int
is_safe(int queens[], int row, int col)
{
  for (int i = 0; i < row; i++)
  {
    int q_col = queens[i];
    if (q_col == col || libmin_abs(q_col - col) == libmin_abs(i - row))
      return FALSE;
  }
  return TRUE;
}

// Recursive backtracking solver
void
solve(int queens[], int row)
{
  if (row == BOARD_SIZE)
  {
    solution_count++;
    return;
  }
  for (int col = 0; col < BOARD_SIZE; col++)
  {
    if (is_safe(queens, row, col))
    {
      queens[row] = col;
      solve(queens, row + 1);
    }
  }
}

int
main(void)
{
  int *queens = (int *)libmin_malloc(BOARD_SIZE * sizeof(int));

  solve(queens, 0);

  libmin_printf("Total solutions for %d-Queens: %d\n", BOARD_SIZE, solution_count);

  libmin_free(queens);
  return 0;
}

