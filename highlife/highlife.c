#include "libmin.h"

#define kRows 20
#define kCols 20
unsigned kIterations = 100 /* 250 */;

uint64_t board_a[kRows][kCols];
uint64_t board_b[kRows][kCols];

typedef uint64_t board_t[kRows][kCols];

inline unsigned wrap_row(int r) {
  return (unsigned)((r + (int)(kRows)) % (int)(kRows));
}

inline unsigned wrap_col(int c) {
  return (unsigned)((c + (int)(kCols)) % (int)(kCols));
}

void print_board(unsigned i, board_t *board) {
  libmin_printf("HighLife %ux%u after %u iterations:\n", kRows, kCols, i);

  unsigned alive_count = 0;
  for (unsigned r = 0; r < kRows; ++r) {
    for (unsigned c = 0; c < kCols; ++c) {
      const uint64_t cell = (*board)[r][c];
      const char ch = cell ? '#' : '.';
      libmin_printf("%c", ch);
      alive_count += (unsigned)(cell);
    }
    libmin_printf("\n");
  }

  libmin_printf("Alive cells: %u\n", alive_count);
}

void init_board() {
  for (unsigned r = 0; r < kRows; ++r) {
    for (unsigned c = 0; c < kCols; ++c) {
      const uint64_t v = libmin_rand() & 1u;
      board_a[r][c] = v;
      board_b[r][c] = 0;
    }
  }
}

uint64_t neighbor_count(board_t board, unsigned r, unsigned c) {
  uint64_t n = 0;
  for (int dr = -1; dr <= 1; ++dr) {
    for (int dc = -1; dc <= 1; ++dc) {
      if (dr == 0 && dc == 0) {
        continue;
      }
      n += board[wrap_row((int)(r) + dr)][wrap_col((int)(c) + dc)];
    }
  }
  return n;
}

void step_highlife(board_t cur, board_t nxt) {
  for (unsigned r = 0; r < kRows; ++r) {
    for (unsigned c = 0; c < kCols; ++c) {
      const uint64_t alive = cur[r][c];
      const uint64_t n = neighbor_count(cur, r, c);
      const uint64_t survive = (n == 2) | (n == 3);
      const uint64_t born = (n == 3) | /* extra highlife birth condition */(n == 6);
      if (alive)
        nxt[r][c] = survive;
      else
        nxt[r][c] = born;
    }
  }
}

board_t *run_simulation() {
  board_t* cur = &board_a;
  board_t* nxt = &board_b;

  for (unsigned i = 0; i < kIterations; ++i) {
    if ((i % 25) == 0)
      print_board(i, cur);
    step_highlife(*cur, *nxt);
    board_t* tmp = cur;
    cur = nxt;
    nxt = tmp;
  }

  return cur;
}

int main(void)
{
  // initialize RNG
  libmin_srand(42);

  init_board();
  board_t *final_board = run_simulation();
  print_board(kIterations, final_board);

  libmin_success();
  return 0;
}
