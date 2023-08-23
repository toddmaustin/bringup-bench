#include "libmin.h"

#include "consttypes.h"

// SOME COMPILERS THROW WARNINGS FOR EXTERN KEYWORD
// BUT MAY BE HANDY TO KEEP IN AS REMINDER, AND NO CONFUSION - SO LONG AS COMPILES

square_t white_initial_squares[ORIGINAL_PIECES_COUNT] = 
	{"B1", "D1", "F1", "H1", "A2", "C2", "E2", "G2", "B3", "D3", "F3", "H3"};
square_t black_initial_squares[ORIGINAL_PIECES_COUNT] = 
	{"A6", "C6", "E6", "G6", "B7", "D7", "F7", "H7", "A8", "C8", "E8", "G8"};
char BLACK[] = "BLACK";
char WHITE[] = "WHITE";
movement_t black_piece_movements[NUM_PIECE_MOVEMENTS] = {{1, -1}, {-1, -1}};
	// vectors for legal movement directions for black piece
movement_t white_piece_movements[NUM_PIECE_MOVEMENTS] = {{1, 1}, {-1, 1}};
	// vectors for legal movement directions for white piece
movement_t tower_movements[NUM_TOWER_MOVEMENTS] = 
	{{1, -1}, {1, 1}, {-1, 1}, {-1 ,-1}};
	// vectors for legal movement directions for tower piece
	// ALL MOVEMENTS ORDERED FROM NE AND ROTATE CLOCKWISE
linked_list_t initial_children = {NULL, NULL};
	// initially each node is childless

