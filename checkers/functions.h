#include "libmin.h"

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void fill_print_initial(board_t* board);
int char_to_col(char col);
int char_to_row(char row);
char col_to_char(int col);
char row_to_char(int row);
void print_board(board_t* board_input);
int check_move_error_1_to_5(board_t board_input, 
	char col1, char row1, char col2, char row2, int black_action);
int outside_of_board(char col, char row);
char piece_at_location(board_t board_input, char col, char row);
void change_board(board_t* board, char col1, char row1, char col2, char row2);
int all_possible_moves(board_t board_input, char col, char row, 
	valid_moves_t valid_moves, int black_action);
int min(int a, int b);
void print_error_message(int error_code);
int board_cost(board_t board_input);
void print_move_information(int generated_move, int black_action, 
	board_t board_input, int col1, int row1, int col2, int row2, int action);
void generate_node_children(tree_node_t* node, int depth);
int movable_checker(int black_action, char checker);
void link_new_node(tree_node_t* parent_node,
	char col1, char row1, char col2, char row2, int depth);
void generate_tree_depth_3(tree_node_t* root_node);
int move_score_forced(tree_node_t* node);
void free_tree(tree_node_t* node);

#endif

