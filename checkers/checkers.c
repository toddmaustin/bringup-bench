#include "libmin.h"

#include "consttypes.h"
#include "functions.h"

#include "test0-txt.h"
MFILE __infile = {
  "test0.txt",
  __test0_sz,
  __test0,
  0
};
MFILE *infile = &__infile;

int
main(int argc, char *argv[])
{
  libmin_mopen(infile, "r");

  board_t* board = (board_t*)libmin_malloc(sizeof(board_t)); // main board pointer

  fill_print_initial(board);
  print_board(board);
    
  int black_action = 1, action = 1, error; //, board_cost;
  char col1, row1, col2, row2; 
	// col1/row1 is source cell,  col2/row2 is target cell
	// deliberately kept separate, square_t type not used
	
	// scan input moves
	char buf[64];
	libmin_mgets(buf, 64, infile);
	while (libmin_sscanf(buf, "%c%c-%c%c\n", &col1, &row1, &col2, &row2)==4) {
		// check errors 1-5, terminate function if any found
		error = check_move_error_1_to_5(*board, col1, row1, col2, row2, 
			black_action);
		if (error) {
			print_error_message(error);
			libmin_fail(error);
		}
		// check error 6
			// all the possible moves of the source cell are generated
			// and the target cell is checked against those
		static valid_moves_t valid_moves;
		int num_possibles = all_possible_moves(*board, col1, row1, valid_moves, 
			black_action);
		int illegal = 1;
		for (int i=0;i<num_possibles;i++) {
			if ((valid_moves[i][0] == col2) && (valid_moves[i][1] == row2)) {
				illegal = 0;
			}
		}
		// if target_cell input is not in all legal moves, it must be illegal
			// return main function with error exit code 6
		if (illegal) {
			print_error_message(6);
			libmin_fail(6);
		}
		
		change_board(board, col1, row1, col2, row2);
		
		print_move_information(0, black_action, *board, col1, row1, col2, row2, 
			action);
		
		print_board(board);
		
		// change the action to opposite colour, iterate the move counts
		black_action = !black_action;
		action += 1;

    // get the next move
    libmin_mgets(buf, 64, infile);
	}
	
	char next_action = col1;
		// on last scanf call, col1 will pickup any trailing P or A instruction
			// naming isn't great, hence a reassignment to better variable name
	
	// stages 1 and 2
	int repititions=0; 
	if (next_action == 'A') { // stage 1
		repititions = 1;
	}
	if (next_action == 'P') { // stage 2
		repititions = 100;
	}
			
	for (int i=0; i<repititions; i++) {
		// stage 1
			// create the level 0 node first, and fill with relevent info.
		tree_node_t* level_0_node = (tree_node_t*)libmin_malloc(sizeof(tree_node_t));
		level_0_node->depth = 0;
		for (int i=0; i<BOARD_SIZE;i++) {
			for (int j=0; j<BOARD_SIZE; j++) {
				(level_0_node->board_state)[i][j] = (*board)[i][j];
			}
		}
		level_0_node->black_action = black_action;
		level_0_node->children_count = 0;
		level_0_node->children_list = initial_children;
		
		generate_tree_depth_3(level_0_node);
		
		// check if there isn't any possible moves, indicating game over
		if (level_0_node->children_count == 0) {
			if (black_action) {
				libmin_printf("%s WIN!\n", WHITE);
        libmin_success();
			}
			else {
				libmin_printf("%s WIN!\n", BLACK);
        libmin_success();
			}
		}
		
		// now we journey into the tree, and implement the minimax decision rule
		int best_score = move_score_forced(level_0_node); 
			// this is the best possible score we can have
		
		// find the choice node which yields this score
		linked_list_member_t* list_member = (level_0_node->children_list).first;
		tree_node_t* choice_node = NULL;
		for (int i=0; i<(level_0_node->children_count); i++) {
			choice_node = (tree_node_t*)(list_member->child_node);
			if (move_score_forced(choice_node) == best_score) {
				break;
			}
			list_member = list_member->next;
		}
		
		change_board(board, 
			(choice_node->move_info)[0][0], 
			(choice_node->move_info)[0][1],
			(choice_node->move_info)[1][0],
			(choice_node->move_info)[1][1]);
		
		print_move_information(1, black_action, *board, 
			(choice_node->move_info)[0][0], 
			(choice_node->move_info)[0][1], 
			(choice_node->move_info)[1][0], 
			(choice_node->move_info)[1][1], 
			action);
		
		print_board(board);
		
		black_action = !black_action;
		action += 1;
		
		free_tree(level_0_node); // free space occupied by the tree
			// no need for it anymore
	}
	
	libmin_free(board);
  libmin_success(); // exit program with the success code
}
// algorithms are fun

