//===--- cnf_reader.h -------------------------------------------------------===
//
//                     satomi: Satisfiability solver
//
// This file is distributed under the BSD 2-Clause License.
// See LICENSE for details.
//
//===------------------------------------------------------------------------===
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "satomi.h"
#include "solver.h"
#include "mem.h"
#include "vec.h"

/** Read the file into an internal buffer.
 *
 * This function will receive a file name. The return data is a string ended
 * with '\0'.
 *
 */
static char *
file_open(const char *fname)
{
	FILE *file = fopen(fname, "rb");
	char *buffer;
	size_t sz_file;

	if (file == NULL) {
		fprintf(stdout, "Couldn't open file: %s\n", fname);
		exit(EXIT_FAILURE);
	}
	fseek(file, 0, SEEK_END);
	sz_file = ftell(file);
	rewind(file);
	buffer = STM_ALLOC(char, sz_file + 3);
	fread(buffer, sz_file, 1, file);
	buffer[sz_file + 0] = '\n';
	buffer[sz_file + 1] = '\0';
	return buffer;
}

static void
skip_spaces(char **pos)
{
	assert(pos != NULL);
	for (; isspace(**pos); (*pos)++);
}

static void
skip_line(char **pos)
{
	assert(pos != NULL);
	for(; **pos != '\n' && **pos != '\r' && **pos != EOF; (*pos)++);
	if (**pos != EOF)
		(*pos)++;
	return;
}

static int
read_int(char **token)
{
	int value = 0;
	int neg = 0;

	skip_spaces(token);
	if (**token == '-') {
		neg = 1;
		(*token)++;
	} else if (**token == '+')
		(*token)++;

	if (!isdigit(**token)) {
		fprintf(stdout, "Parsing error. Unexpected char: %c.\n", **token);
		exit(EXIT_FAILURE);
	}
	while (isdigit(**token)) {
		value = (value * 10) + (**token - '0');
		(*token)++;
	}
	return neg ? -value : value;
}

static void
read_clause(char **token, vec_ui32_t *lits)
{
	int var;
	uint32_t sign;

	vec_clear(lits);
	while (1) {
		var = read_int(token);
		if (var == 0)
			break;
		sign = (var > 0);
		var = abs(var) - 1;
		vec_push_back(lits, var2lit((uint32_t) var, !sign));
	}
}

/** Start the solver and reads the DIMAC file.
 *
 * Returns false upon immediate conflict.
 */
int
satomi_parse_dimacs(char *fname, satomi_t **solver)
{
	satomi_t *p = NULL;
	vec_ui32_t *lits = NULL;
	int n_var;
	int n_clause;
	char *token = file_open(fname);
	char *name = strrchr(fname, '/') + 1;

	while (1) {
		skip_spaces(&token);
		if (*token == 0)
			break;
		else if (*token == 'c')
			skip_line(&token);
		else if (*token == 'p') {
			token++;
			skip_spaces(&token);
			for(; !isspace(*token); token++); /* skip 'cnf' */

			n_var = read_int(&token);
			n_clause = read_int(&token); (void)n_clause;
			skip_line(&token);
			lits = vec_ui32_alloc((uint32_t) n_var);
			p = satomi_create(name);
		} else {
			if (lits == NULL) {
				fprintf(stdout, "There is no parameter line.\n");
				return -1;
			}
			read_clause(&token, lits);
			if (!satomi_add_clause(p, vec_data(lits), vec_size(lits))) {
				vec_print(lits);
				return 0;
			}
        	}
	}
	vec_free(lits);
	*solver = p;
	return SATOMI_OK;
}

