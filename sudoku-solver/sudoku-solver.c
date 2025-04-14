#include "libmin.h"

#define N 9  // Sudoku grid size (9x9)

// Hard, but solvable, Sudoku puzzle.
// Zeros indicate empty cells.
static int board[N][N] = {
    {8, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 6, 0, 0, 0, 0, 0},
    {0, 7, 0, 0, 9, 0, 2, 0, 0},
    {0, 5, 0, 0, 0, 7, 0, 0, 0},
    {0, 0, 0, 0, 4, 5, 7, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 3, 0},
    {0, 0, 1, 0, 0, 0, 0, 6, 8},
    {0, 0, 8, 5, 0, 0, 0, 1, 0},
    {0, 9, 0, 0, 0, 0, 4, 0, 0}
};

// Check if placing num at board[row][col] is valid.
int isSafe(int row, int col, int num) {
    // Check row for duplicates.
    for (int i = 0; i < N; i++) {
        if (board[row][i] == num)
            return 0;
    }
    // Check column for duplicates.
    for (int i = 0; i < N; i++) {
        if (board[i][col] == num)
            return 0;
    }
    // Check 3x3 subgrid for duplicates.
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[startRow + i][startCol + j] == num)
                return 0;
        }
    }
    return 1;
}

// Recursively attempt to fill the Sudoku board.
int solveSudoku() {
    int row = -1;
    int col = -1;
    int emptyFound = 0;

    // Find an empty cell.
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 0) {
                row = i;
                col = j;
                emptyFound = 1;
                break;
            }
        }
        if (emptyFound)
            break;
    }

    // No empty cell found; puzzle is solved.
    if (!emptyFound)
        return 1;

    // Try placing numbers 1 to 9 in the empty cell.
    for (int num = 1; num <= 9; num++) {
        if (isSafe(row, col, num)) {
            board[row][col] = num;
            if (solveSudoku())
                return 1;
            board[row][col] = 0;  // Backtrack.
        }
    }
    return 0;  // Trigger backtracking.
}

// Utility function to print the Sudoku board.
void printBoard() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            libmin_printf("%d ", board[i][j]);
        }
        libmin_printf("\n");
    }
}

int main() {
    libmin_printf("Initial Sudoku Puzzle:\n");
    printBoard();

    if (solveSudoku()) {
        libmin_printf("\nSolved Sudoku Puzzle:\n");
        printBoard();
        libtarg_success();
    } else {
        libmin_printf("No solution found.\n");
        libtarg_fail(1);
    }
    return 0;
}

