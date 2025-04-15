#include "libmin.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

// Heuristic weights (tuned values from research)
#define WEIGHT_AGGREGATE_HEIGHT -0.510066
#define WEIGHT_COMPLETE_LINES    0.760666
#define WEIGHT_HOLES            -0.35663
#define WEIGHT_BUMPINESS        -0.184483

// ------------------------
// Data Structures
// ------------------------

typedef struct {
    int x, y;
} Point;

typedef struct {
    int numBlocks;    // In Tetris, every piece is 4 blocks.
    Point blocks[4];
} Orientation;

typedef struct {
    char name[3];     // e.g., "I", "O", etc.
    int numOrientations;
    const Orientation *orientations;
} Piece;

// ------------------------
// Tetromino Definitions
// ------------------------

// I-piece: two orientations
static const Orientation I_orientations[] = {
    {4, {{0,0}, {1,0}, {2,0}, {3,0}}},
    {4, {{0,0}, {0,1}, {0,2}, {0,3}}}
};

// O-piece: one orientation
static const Orientation O_orientations[] = {
    {4, {{0,0}, {1,0}, {0,1}, {1,1}}}
};

// T-piece: four orientations
static const Orientation T_orientations[] = {
    {4, {{0,0}, {-1,0}, {1,0}, {0,1}}},
    {4, {{0,0}, {0,-1}, {0,1}, {1,0}}},
    {4, {{0,0}, {-1,0}, {1,0}, {0,-1}}},
    {4, {{0,0}, {0,-1}, {0,1}, {-1,0}}}
};

// S-piece: two orientations
static const Orientation S_orientations[] = {
    {4, {{0,0}, {1,0}, {0,1}, {-1,1}}},
    {4, {{0,0}, {0,-1}, {1,0}, {1,1}}}
};

// Z-piece: two orientations
static const Orientation Z_orientations[] = {
    {4, {{0,0}, {-1,0}, {0,1}, {1,1}}},
    {4, {{0,0}, {0,-1}, {-1,0}, {-1,1}}}
};

// J-piece: four orientations (approximate definitions)
static const Orientation J_orientations[] = {
    {4, {{0,0}, {0,1}, {0,2}, {-1,2}}},
    {4, {{0,0}, {1,0}, {2,0}, {2,1}}},
    {4, {{0,0}, {1,0}, {0,1}, {0,2}}},
    {4, {{0,0}, {0,1}, {-1,1}, {-2,1}}}
};

// L-piece: four orientations (approximate definitions)
static const Orientation L_orientations[] = {
    {4, {{0,0}, {0,1}, {0,2}, {1,2}}},
    {4, {{0,0}, {1,0}, {2,0}, {0,1}}},
    {4, {{0,0}, {1,0}, {1,1}, {1,2}}},
    {4, {{2,0}, {0,1}, {1,1}, {2,1}}}
};

static const Piece pieces[] = {
    {"I", 2, I_orientations},
    {"O", 1, O_orientations},
    {"T", 4, T_orientations},
    {"S", 2, S_orientations},
    {"Z", 2, Z_orientations},
    {"J", 4, J_orientations},
    {"L", 4, L_orientations}
};

#define NUM_PIECES (sizeof(pieces)/sizeof(Piece))

// ------------------------
// Board and Utility Functions
// ------------------------

// Create an empty board (fill with zeros)
void create_board(int board[BOARD_HEIGHT][BOARD_WIDTH]) {
    for (int y = 0; y < BOARD_HEIGHT; y++)
        for (int x = 0; x < BOARD_WIDTH; x++)
            board[y][x] = 0;
}

// Check whether placing an orientation at (offset_x, offset_y) causes a collision.
int check_collision(int board[BOARD_HEIGHT][BOARD_WIDTH], const Orientation *ori, int offset_x, int offset_y) {
    for (int i = 0; i < ori->numBlocks; i++) {
        int x = offset_x + ori->blocks[i].x;
        int y = offset_y + ori->blocks[i].y;
        if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT)
            return 1;
        if (board[y][x] != 0)
            return 1;
    }
    return 0;
}

// Place the piece on the board (modifies board)
void place_piece(int board[BOARD_HEIGHT][BOARD_WIDTH], const Orientation *ori, int offset_x, int offset_y) {
    for (int i = 0; i < ori->numBlocks; i++) {
        int x = offset_x + ori->blocks[i].x;
        int y = offset_y + ori->blocks[i].y;
        board[y][x] = 1;
    }
}

// Clear full lines from the board.
// Returns the number of lines cleared.
int clear_lines(int board[BOARD_HEIGHT][BOARD_WIDTH]) {
    int lines_cleared = 0;
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        int full = 1;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board[y][x] == 0) {
                full = 0;
                break;
            }
        }
        if (full) {
            lines_cleared++;
            // Move all rows above down one row.
            for (int yy = y; yy > 0; yy--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    board[yy][x] = board[yy-1][x];
                }
            }
            // Clear the top row.
            for (int x = 0; x < BOARD_WIDTH; x++) {
                board[0][x] = 0;
            }
            y++; // Recheck this row since new content was shifted down.
        }
    }
    return lines_cleared;
}

// ------------------------
// Heuristic Evaluation Functions
// ------------------------

// Compute the height of each column.
void get_column_heights(int board[BOARD_HEIGHT][BOARD_WIDTH], int heights[BOARD_WIDTH]) {
    for (int x = 0; x < BOARD_WIDTH; x++) {
        heights[x] = 0;
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            if (board[y][x] != 0) {
                heights[x] = BOARD_HEIGHT - y;
                break;
            }
        }
    }
}

int aggregate_height(int heights[BOARD_WIDTH]) {
    int sum = 0;
    for (int i = 0; i < BOARD_WIDTH; i++)
        sum += heights[i];
    return sum;
}

int count_holes(int board[BOARD_HEIGHT][BOARD_WIDTH]) {
    int holes = 0;
    for (int x = 0; x < BOARD_WIDTH; x++) {
        int block_found = 0;
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            if (board[y][x] != 0)
                block_found = 1;
            else if (block_found)
                holes++;
        }
    }
    return holes;
}

int bumpiness(int heights[BOARD_WIDTH]) {
    int bump = 0;
    for (int i = 0; i < BOARD_WIDTH - 1; i++)
        bump += libmin_abs(heights[i] - heights[i+1]);
    return bump;
}

// Evaluate a board state given the number of lines cleared by the last move.
double evaluate_board(int board[BOARD_HEIGHT][BOARD_WIDTH], int lines_cleared) {
    int heights[BOARD_WIDTH];
    get_column_heights(board, heights);
    int agg = aggregate_height(heights);
    int holes = count_holes(board);
    int bump = bumpiness(heights);
    double score = WEIGHT_AGGREGATE_HEIGHT * agg +
                   WEIGHT_COMPLETE_LINES * lines_cleared +
                   WEIGHT_HOLES * holes +
                   WEIGHT_BUMPINESS * bump;
    return score;
}

// ------------------------
// Simulation: Try dropping a piece and evaluating the resulting board.
// ------------------------

// Simulate dropping the piece (of a given orientation) at horizontal offset 'offset_x'.
// The result is written into sim_board, and the number of lines cleared is returned via lines_cleared.
// Returns 1 if placement is valid, 0 otherwise.
int simulate_board(int orig_board[BOARD_HEIGHT][BOARD_WIDTH], const Orientation *ori, int offset_x, int sim_board[BOARD_HEIGHT][BOARD_WIDTH], int *lines_cleared) {
    libmin_memcpy(sim_board, orig_board, sizeof(int) * BOARD_HEIGHT * BOARD_WIDTH);
    int y = 0;
    // Drop the piece until a collision is detected.
    while (!check_collision(sim_board, ori, offset_x, y))
        y++;
    y--;  // last valid position
    if (y < 0)
        return 0;
    place_piece(sim_board, ori, offset_x, y);
    *lines_cleared = clear_lines(sim_board);
    return 1;
}

// ------------------------
// AI: Find the best move for a given piece on the current board.
// ------------------------
int best_move(const int board[BOARD_HEIGHT][BOARD_WIDTH], const Piece *piece, const Orientation **best_ori, int *best_offset, double *best_score, int best_board[BOARD_HEIGHT][BOARD_WIDTH], int *lines_cleared_out) {
    double max_score = -1e9;
    int move_found = 0;
    int temp_board[BOARD_HEIGHT][BOARD_WIDTH];
    int lines;
    // For each orientation of the piece.
    for (int o = 0; o < piece->numOrientations; o++) {
        const Orientation *ori = &(piece->orientations[o]);
        // Calculate the horizontal span of the orientation.
        int min_x = 100, max_x = -100;
        for (int i = 0; i < ori->numBlocks; i++) {
            int bx = ori->blocks[i].x;
            if (bx < min_x) min_x = bx;
            if (bx > max_x) max_x = bx;
        }
        // Determine valid horizontal offsets so that the piece remains within the board.
        int start = -min_x;
        int end = BOARD_WIDTH - max_x;
        for (int x = start; x < end; x++) {
            if (simulate_board((int (*)[BOARD_WIDTH])board, ori, x, temp_board, &lines)) {
                double score = evaluate_board(temp_board, lines);
                if (score > max_score) {
                    max_score = score;
                    *best_score = score;
                    *best_ori = ori;
                    *best_offset = x;
                    libmin_memcpy(best_board, temp_board, sizeof(int)*BOARD_HEIGHT*BOARD_WIDTH);
                    *lines_cleared_out = lines;
                    move_found = 1;
                }
            }
        }
    }
    return move_found;
}

// ------------------------
// Board Display (text-based)
// ------------------------
void print_board(int board[BOARD_HEIGHT][BOARD_WIDTH]) {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            libmin_printf("%c", board[y][x] ? 'X' : '.');
        }
        libmin_printf("\n");
    }
    libmin_printf("\n");
}

// ------------------------
// Random Piece Selection
// ------------------------
const Piece* random_piece() {
    int index = libmin_rand() % NUM_PIECES;
    return &pieces[index];
}

// ------------------------
// Main Simulation Loop
// ------------------------
int main(void) {
    libmin_srand(42);
    int board[BOARD_HEIGHT][BOARD_WIDTH];
    create_board(board);
    int move_count = 0;
    int total_lines_cleared = 0;

    while (1) {
        const Piece *piece = random_piece();
        const Orientation *best_ori;
        int best_offset;
        double best_score;
        int new_board[BOARD_HEIGHT][BOARD_WIDTH];
        int lines;
        if (!best_move(board, piece, &best_ori, &best_offset, &best_score, new_board, &lines)) {
            libmin_printf("Game over!\n");
            break;
        }
        // Update board state.
        libmin_memcpy(board, new_board, sizeof(board));
        total_lines_cleared += lines;
        move_count++;
        // Print board every 10 moves.
        if (move_count % 10 == 0) {
            libmin_printf("After move %d, total lines cleared: %d, last move score: %f\n", move_count, total_lines_cleared, best_score);
            print_board(board);
        }
    }
    libmin_printf("Game over after %d moves, total lines cleared: %d\n", move_count, total_lines_cleared);
    print_board(board);

    libmin_success();
    return 0;
}

