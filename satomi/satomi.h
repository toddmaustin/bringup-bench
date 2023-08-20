//===--- satomi.h -----------------------------------------------------------===
//
//                     satomi: Satisfiability solver
//
// This file is distributed under the BSD 2-Clause License.
// See LICENSE for details.
//
//===------------------------------------------------------------------------===
#ifndef SATOMI__SATOMI_H
#define SATOMI__SATOMI_H

#include "libmin.h"

/** Return valeus */
enum {
	SATOMI_ERR = 0,
	SATOMI_OK  = 1
};

enum {
	SATOMI_UNDEC = 0, /* Undecided */
	SATOMI_SAT   = 1,
	SATOMI_UNSAT = -1
};

struct solver_t_;
typedef struct solver_t_ satomi_t;

typedef struct satomi_opts satomi_opts_t;
struct satomi_opts {
	char verbose;
};

struct satomi_stats {
	uint64_t n_decisions;
	uint64_t n_propagations;
	uint64_t n_inspects;
	uint64_t n_conflicts;

	uint64_t n_lits;
	double init_time;
};

//===------------------------------------------------------------------------===
extern satomi_t *satomi_create(char *);
extern void satomi_destroy(satomi_t *);
extern void satomi_default_opts(satomi_opts_t *);
extern void satomi_configure(satomi_t *, satomi_opts_t *);
extern int  satomi_parse_dimacs(MFILE *, satomi_t **);
extern void satomi_add_variable(satomi_t *);
extern int  satomi_add_clause(satomi_t *, uint32_t *, uint32_t);
extern int  satomi_solve(satomi_t *);

extern void satomi_print_stats(satomi_t *);
extern void satomi_print_clauses(satomi_t *);

#endif /* SATOMI__SATOMI_H */
