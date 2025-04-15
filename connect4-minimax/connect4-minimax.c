#include "libmin.h"

#define ROWS 6
#define COLS 7
#define MAX_DEPTH 4

// Players: 1 and 2. Board cells: 0 = empty.
#define EMPTY 0

// Scoring constants for evaluation
#define WIN_SCORE 100000
#define LOSS_SCORE -100000

// -------------------------------------------------
// Board Helpers
// -------------------------------------------------
void init_board(int board[ROWS][COLS]) {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            board[r][c] = EMPTY;
}

void print_board(int board[ROWS][COLS]) {
    libmin_printf("\n");
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            char ch = board[r][c] == 1 ? 'X' : (board[r][c] == 2 ? 'O' : '.');
            libmin_printf("%c ", ch);
        }
        libmin_printf("\n");
    }
    for (int c = 0; c < COLS; c++) {
        libmin_printf("%d ", c);
    }
    libmin_printf("\n\n");
}

// Returns 1 if the column is not full.
int valid_move(int board[ROWS][COLS], int col) {
    return board[0][col] == EMPTY;
}

// Returns the lowest available row in the given column (or -1 if full).
int get_next_open_row(int board[ROWS][COLS], int col) {
    for (int r = ROWS - 1; r >= 0; r--) {
        if (board[r][col] == EMPTY)
            return r;
    }
    return -1;
}

// Make a move: drop piece for player into col. Returns row where piece lands, or -1 if invalid.
int make_move(int board[ROWS][COLS], int col, int player) {
    if (!valid_move(board, col))
        return -1;
    int row = get_next_open_row(board, col);
    board[row][col] = player;
    return row;
}

// Undo a move in the given column (remove the top piece).
void undo_move(int board[ROWS][COLS], int col) {
    for (int r = 0; r < ROWS; r++) {
        if (board[r][col] != EMPTY) {
            board[r][col] = EMPTY;
            break;
        }
    }
}

// Check if board is full.
int board_full(int board[ROWS][COLS]) {
    for (int c = 0; c < COLS; c++)
        if (valid_move(board, c))
            return 0;
    return 1;
}

// -------------------------------------------------
// Game Evaluation: Check for win.
// -------------------------------------------------

// Check horizontal, vertical, and two diagonals for four in a row.
int check_win(int board[ROWS][COLS], int player) {
    // Horizontal check
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            if (board[r][c] == player && board[r][c+1] == player &&
                board[r][c+2] == player && board[r][c+3] == player)
                return 1;
        }
    }
    // Vertical check
    for (int c = 0; c < COLS; c++) {
        for (int r = 0; r < ROWS - 3; r++) {
            if (board[r][c] == player && board[r+1][c] == player &&
                board[r+2][c] == player && board[r+3][c] == player)
                return 1;
        }
    }
    // Diagonal (positive slope)
    for (int r = 3; r < ROWS; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            if (board[r][c] == player && board[r-1][c+1] == player &&
                board[r-2][c+2] == player && board[r-3][c+3] == player)
                return 1;
        }
    }
    // Diagonal (negative slope)
    for (int r = 0; r < ROWS - 3; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            if (board[r][c] == player && board[r+1][c+1] == player &&
                board[r+2][c+2] == player && board[r+3][c+3] == player)
                return 1;
        }
    }
    return 0;
}

// -------------------------------------------------
// Heuristic Evaluation Function
// -------------------------------------------------

// Evaluate a window of 4 cells for scoring.
int evaluate_window(int window[4], int player) {
    int score = 0;
    int opp = (player == 1) ? 2 : 1;
    int count_player = 0, count_opp = 0, count_empty = 0;
    for (int i = 0; i < 4; i++) {
        if (window[i] == player) count_player++;
        else if (window[i] == opp) count_opp++;
        else count_empty++;
    }
    if (count_player == 4)
        score += 100;
    else if (count_player == 3 && count_empty == 1)
        score += 5;
    else if (count_player == 2 && count_empty == 2)
        score += 2;
    if (count_opp == 3 && count_empty == 1)
        score -= 4;
    return score;
}

// Score the board from the perspective of "player".
int score_position(int board[ROWS][COLS], int player) {
    int score = 0;
    int window[4];
    int center_count = 0;
    // Score center column: pieces in the center are more valuable.
    for (int r = 0; r < ROWS; r++) {
        if (board[r][COLS/2] == player)
            center_count++;
    }
    score += center_count * 3;

    // Horizontal
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            for (int i = 0; i < 4; i++)
                window[i] = board[r][c+i];
            score += evaluate_window(window, player);
        }
    }
    // Vertical
    for (int c = 0; c < COLS; c++) {
        for (int r = 0; r < ROWS - 3; r++) {
            for (int i = 0; i < 4; i++)
                window[i] = board[r+i][c];
            score += evaluate_window(window, player);
        }
    }
    // Positive diagonal
    for (int r = 3; r < ROWS; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            for (int i = 0; i < 4; i++)
                window[i] = board[r-i][c+i];
            score += evaluate_window(window, player);
        }
    }
    // Negative diagonal
    for (int r = 0; r < ROWS - 3; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            for (int i = 0; i < 4; i++)
                window[i] = board[r+i][c+i];
            score += evaluate_window(window, player);
        }
    }
    return score;
}

// Check if terminal state has been reached and return a score.
// If player wins, return WIN_SCORE; if opponent wins, return LOSS_SCORE; if draw return 0.
int terminal_score(int board[ROWS][COLS], int player) {
    int opp = (player == 1) ? 2 : 1;
    if (check_win(board, player))
        return WIN_SCORE;
    if (check_win(board, opp))
        return LOSS_SCORE;
    return 0;
}

// -------------------------------------------------
// Minimax with Alpha-Beta Pruning and Random Tie-Breaking
// -------------------------------------------------

// Returns evaluation score. If at top level, best_move will be set (column index).
int minimax(int board[ROWS][COLS], int depth, int alpha, int beta, int maximizingPlayer, int player, int *best_move) {
    int opp = (player == 1) ? 2 : 1;
    // Check terminal conditions or depth limit
    if (depth == 0 || board_full(board) || check_win(board, player) || check_win(board, opp)) {
        int ts = terminal_score(board, player);
        if (ts == 0) { // non-terminal leaf at depth 0: use heuristic
            return score_position(board, player);
        } else {
            return ts;
        }
    }
    
    int valid_cols[COLS];
    int valid_count = 0;
    for (int c = 0; c < COLS; c++) {
        if (valid_move(board, c))
            valid_cols[valid_count++] = c;
    }
    
    int best_score;
    // For random tie-breaking, we keep an array of moves with best scores.
    int best_moves[COLS];
    int best_moves_count = 0;
    
    if (maximizingPlayer) {
        best_score = INT_MIN;
        for (int i = 0; i < valid_count; i++) {
            int col = valid_cols[i];
            make_move(board, col, player);
            int score = minimax(board, depth - 1, alpha, beta, 0, player, NULL);
            undo_move(board, col);
            // Random tie breaking: if equal score, record all such moves.
            if (score > best_score) {
                best_score = score;
                best_moves_count = 0;
                best_moves[best_moves_count++] = col;
            } else if (score == best_score) {
                best_moves[best_moves_count++] = col;
            }
            if (best_score > alpha)
                alpha = best_score;
            if (alpha >= beta)
                break;
        }
    } else {
        best_score = INT_MAX;
        for (int i = 0; i < valid_count; i++) {
            int col = valid_cols[i];
            make_move(board, col, opp);
            int score = minimax(board, depth - 1, alpha, beta, 1, player, NULL);
            undo_move(board, col);
            if (score < best_score) {
                best_score = score;
                best_moves_count = 0;
                best_moves[best_moves_count++] = col;
            } else if (score == best_score) {
                best_moves[best_moves_count++] = col;
            }
            if (best_score < beta)
                beta = best_score;
            if (alpha >= beta)
                break;
        }
    }
    
    // At top level, choose a random move from the best moves if available.
    if (depth == MAX_DEPTH && best_moves_count > 0 && best_move != NULL) {
        int chosen = best_moves[libmin_rand() % best_moves_count];
        *best_move = chosen;
    }
    return best_score;
}

// -------------------------------------------------
// Main Simulation Loop: Self-play game using minimax
// -------------------------------------------------
void play_game() {
    int board[ROWS][COLS];
    init_board(board);
    
    int current_player = 1; // Player 1 starts.
    int move_number = 0;
    
    print_board(board);
    
    while (1) {
        move_number++;
        int best_move = -1;
        // For both players, we use minimax to choose a move.
        int score = minimax(board, MAX_DEPTH, INT_MIN, INT_MAX, 1, current_player, &best_move);
        if (best_move == -1 || !valid_move(board, best_move)) {
            // No valid move found => game over.
            break;
        }
        make_move(board, best_move, current_player);
        libmin_printf("Move %d: Player %d drops in column %d (score=%d)\n", move_number, current_player, best_move, score);
        print_board(board);
        
        if (check_win(board, current_player)) {
            libmin_printf("Player %d wins!\n", current_player);
            break;
        }
        if (board_full(board)) {
            libmin_printf("Game is a draw.\n");
            break;
        }
        // Switch players.
        current_player = (current_player == 1) ? 2 : 1;
    }
}

int main(void) {
    libmin_srand(42);
    libmin_printf("Connect Four: Minimax AI Self-Play\n");
    play_game();

    libmin_success();
    return 0;
}

