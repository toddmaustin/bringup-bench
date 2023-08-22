#include <stdlib.h>
#include <stdio.h>

#include "consttypes.h"

// SOME COMPILERS THROW WARNINGS FOR EXTERN KEYWORD
// BUT MAY BE HANDY TO KEEP IN AS REMINDER, AND NO CONFUSION - SO LONG AS COMPILES

extern square_t white_initial_squares[ORIGINAL_PIECES_COUNT] = 
	{"B1", "D1", "F1", "H1", "A2", "C2", "E2", "G2", "B3", "D3", "F3", "H3"};
extern square_t black_initial_squares[ORIGINAL_PIECES_COUNT] = 
	{"A6", "C6", "E6", "G6", "B7", "D7", "F7", "H7", "A8", "C8", "E8", "G8"};
extern char BLACK[] = "BLACK";
extern char WHITE[] = "WHITE";
extern movement_t black_piece_movements[NUM_PIECE_MOVEMENTS] = {{1, -1}, {-1, -1}};
	// vectors for legal movement directions for black piece
extern movement_t white_piece_movements[NUM_PIECE_MOVEMENTS] = {{1, 1}, {-1, 1}};
	// vectors for legal movement directions for white piece
extern movement_t tower_movements[NUM_TOWER_MOVEMENTS] = 
	{{1, -1}, {1, 1}, {-1, 1}, {-1 ,-1}};
	// vectors for legal movement directions for tower piece
	// ALL MOVEMENTS ORDERED FROM NE AND ROTATE CLOCKWISE
extern linked_list_t initial_children = {NULL, NULL};
	// initially each node is childless

