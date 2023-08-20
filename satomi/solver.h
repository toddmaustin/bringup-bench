//===--- solver.h -----------------------------------------------------------===
//
//                     satomi: Satisfiability solver
//
// This file is distributed under the BSD 2-Clause License.
// See LICENSE for details.
//
//===------------------------------------------------------------------------===
#ifndef SATOMI__SOLVER_H
#define SATOMI__SOLVER_H

#include "libmin.h"
#include "cdb.h"
#include "clause.h"
#include "satomi.h"
#include "watch_list.h"
#include "mem.h"
#include "misc.h"
#include "vec.h"

enum {
	LIT_FALSE = 1,
	LIT_TRUE = 0,
	VAR_UNASSING = 3
};

#define UNDEF 0xFFFFFFFF

typedef struct solver_t_ solver_t;
struct solver_t_ {
	/* Input info */
	char *fname; 
	
	/* Clauses Database */
	vec_ui32_t *clauses;
	struct cdb *clause_db;
	vec_wl_t *watches;

	/* Variable Information */
	vec_ui32_t *var_order;
	vec_ui32_t *levels;
	vec_ui32_t *reasons;
	vec_ui8_t *assigns;
	vec_ui8_t *polarity;

	/* Assignments */
	vec_ui32_t *trail;
	vec_ui32_t *trail_lim;
	uint32_t i_qhead;

	/* Temporary data */
	vec_ui32_t *temp_lits;
	vec_ui8_t *seen;
	vec_ui32_t *tagged;
	vec_ui32_t *stack;
	vec_ui32_t *last_dlevel;

	struct satomi_stats stats;
	struct satomi_opts opts;
};

//===------------------------------------------------------------------------===
extern uint32_t solver_clause_create(solver_t *, vec_ui32_t *);
extern int solver_search(solver_t *);
extern uint32_t solver_propagate(solver_t *);

//===------------------------------------------------------------------------===
// Inline var/lit functions
//===------------------------------------------------------------------------===
static inline uint32_t 
var2lit(uint32_t var, uint8_t pol) { return var + var + ((uint32_t) pol != 0); }

static inline uint32_t
lit2var(uint32_t lit) { return lit >> 1; }
//===------------------------------------------------------------------------===
// Inline var functions
//===------------------------------------------------------------------------===
static inline uint8_t
var_value(solver_t *s, uint32_t var) { return vec_at(s->assigns, var); }

static inline uint32_t
var_dlevel(solver_t *s, uint32_t var) { return vec_at(s->levels, var); }

static inline uint32_t
var_reason(solver_t *s, uint32_t var) { return vec_at(s->reasons, var); }
//===------------------------------------------------------------------------===
// Inline lit functions
//===------------------------------------------------------------------------===
static inline uint32_t
lit_neg(uint32_t lit) { return lit ^ 1; }

static inline uint8_t
lit_polarity(uint32_t lit) { return (uint8_t)(lit & 1); }

static inline uint8_t
lit_value(solver_t *s, uint32_t lit)
{
	return lit_polarity(lit) ^ vec_at(s->assigns, lit2var(lit));
}

static inline uint32_t
lit_dlevel(solver_t *s, uint32_t lit) { return vec_at(s->levels, lit2var(lit)); }

static inline uint32_t
lit_reason(solver_t *s, uint32_t lit) { return vec_at(s->reasons, lit2var(lit)); }
//===------------------------------------------------------------------------===
// Inline solver minor functions
//===------------------------------------------------------------------------===
static inline uint32_t 
solver_dlevel(solver_t *s) { return vec_size(s->trail_lim); }

static inline uint32_t 
solver_last_decision(solver_t *s)
{
	return vec_at(s->trail, vec_at(s->trail_lim, vec_size(s->trail_lim) - 1));
}

static inline int
solver_enqueue(solver_t *s, uint32_t lit, uint32_t reason)
{
	uint32_t var = lit2var(lit);

	vec_assign(s->assigns, var, lit_polarity(lit));
	vec_assign(s->levels, var, solver_dlevel(s));
	vec_assign(s->reasons, var, reason);
	vec_push_back(s->trail, lit);
	return SATOMI_OK;
}

//===------------------------------------------------------------------------===
// Inline clause functions
//===------------------------------------------------------------------------===
static inline struct clause *
clause_read(solver_t *s, uint32_t cref)
{
	return cdb_handler(s->clause_db, cref);
}

static inline void
clause_watch(solver_t *s, uint32_t cref)
{
	struct clause *clause = cdb_handler(s->clause_db, cref);
	struct watcher w1;
	struct watcher w2;

	w1.cref = cref;
	w2.cref = cref;
	w1.blocker = clause->lits[1];
	w2.blocker = clause->lits[0];
	watch_list_push(vec_wl_at(s->watches, lit_neg(clause->lits[0])), w1, (clause->size == 2));
	watch_list_push(vec_wl_at(s->watches, lit_neg(clause->lits[1])), w2, (clause->size == 2));
}

static inline void
clause_unwatch(solver_t *s, uint32_t cref)
{
	struct clause *clause = cdb_handler(s->clause_db, cref);
	watch_list_remove(vec_wl_at(s->watches, lit_neg(clause->lits[0])), cref, (clause->size == 2));
	watch_list_remove(vec_wl_at(s->watches, lit_neg(clause->lits[1])), cref, (clause->size == 2));
}

#endif /* SATOMI__SOLVER_H */

