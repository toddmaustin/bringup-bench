
#include "libmin.h"
 
// sizes support 10 (default), 15, 20, 25
#define SIZE 10

// `M × N` matrix
#define M SIZE
#define N SIZE

// matrix showing portion of the screen having different colors
char mat[M][N] =
{
#if SIZE == 10
  { 'Y', 'Y', 'Y', 'G', 'G', 'G', 'G', 'G', 'G', 'G' },
  { 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'G', 'X', 'X', 'X' },
  { 'G', 'X', 'G', 'G', 'G', 'G', 'G', 'X', 'X', 'X' },
  { 'W', 'X', 'X', 'W', 'W', 'G', 'G', 'G', 'G', 'X' },
  { 'W', 'R', 'R', 'R', 'R', 'R', 'G', 'X', 'X', 'X' },
  { 'W', 'W', 'W', 'R', 'R', 'G', 'G', 'X', 'X', 'X' },
  { 'W', 'B', 'W', 'R', 'R', 'R', 'R', 'R', 'R', 'X' },
  { 'W', 'B', 'B', 'B', 'B', 'R', 'R', 'X', 'X', 'X' },
  { 'W', 'B', 'B', 'X', 'B', 'B', 'B', 'B', 'X', 'X' },
  { 'W', 'B', 'B', 'X', 'X', 'X', 'X', 'X', 'X', 'X' }
#elif SIZE == 15
  { 'Y', 'Y', 'Y', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'G', 'X', 'G', 'G', 'G', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'X', 'X', 'W', 'W', 'G', 'G', 'G', 'G', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'R', 'R', 'R', 'R', 'R', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'W', 'W', 'R', 'R', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'W', 'R', 'R', 'R', 'R', 'R', 'R', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'B', 'B', 'R', 'R', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'B', 'B', 'B', 'B', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
#elif SIZE == 20
  { 'Y', 'Y', 'Y', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'G', 'X', 'G', 'G', 'G', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'X', 'X', 'W', 'W', 'G', 'G', 'G', 'G', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'R', 'R', 'R', 'R', 'R', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'W', 'W', 'R', 'R', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'W', 'R', 'R', 'R', 'R', 'R', 'R', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'B', 'B', 'R', 'R', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'B', 'B', 'B', 'B', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
#elif SIZE == 25
  { 'Y', 'Y', 'Y', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'G', 'X', 'G', 'G', 'G', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'X', 'X', 'W', 'W', 'G', 'G', 'G', 'G', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'R', 'R', 'R', 'R', 'R', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'W', 'W', 'R', 'R', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'W', 'R', 'R', 'R', 'R', 'R', 'R', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'B', 'B', 'R', 'R', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'B', 'B', 'B', 'B', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
#endif
};
 
// Below arrays detail all eight possible movements
int row[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int col[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
 
// check if it is possible to go to pixel `(x, y)` from the
// current pixel. The function returns false if the pixel
// has a different color, or it's not a valid pixel
#define SAFELOC(X, Y)     ((X) >= 0 && (X) < M && (Y) >= 0 && (Y) < N)
#define ISSAFE(MAT, X, Y, TARGET)     (((X) >= 0 && (X) < M && (Y) >= 0 && (Y) < N) && mat[(X)][(Y)] == (TARGET))
 
// Flood fill using DFS
void
floodfill(char mat[M][N], char x, char y, char replacement)
{
  // get the target color
  char target = mat[(int)x][(int)y];
 
  // replace the current pixel color with that of replacement
  mat[(int)x][(int)y] = replacement;
 
  // process all eight adjacent pixels of the current pixel and
  // recur for each valid pixel
  for (int k = 0; k < 8; k++)
  {
    // if the adjacent pixel at position `(x + row[k], y + col[k])` is
    // a valid pixel and has the same color as that of the current pixel
    if (ISSAFE(mat, x + row[k], y + col[k], target))
    {
      floodfill(mat, x + row[k], y + col[k], replacement);
    }
  }
}
 
// Utility function to print a matrix
void
printMatrix(char mat[M][N])
{
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
      libmin_printf("  %c", mat[i][j]);
    libmin_printf("\n");
  }
}
 
int
main()
{

  // start node
  int x = 3, y = 9;    // having a target color `X`

  // replacement color, NOTE: must be a new color!
  char replacement = 'C';

  // print the colors before replacement
  libmin_printf("\nBEFORE flooding `%c' @ (%d,%d):\n", replacement, x, y); printMatrix(mat);

  // replace the target color with a replacement color using DFS
  floodfill(mat, x, y, replacement);

  // print the colors after replacement
  libmin_printf("\nAFTER:\n"); printMatrix(mat);

  libmin_success();
  return 0;
}

