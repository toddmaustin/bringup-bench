//===--- solver.c -----------------------------------------------------------===
//
//                     satomi: Satisfiability solver
//
// This file is distributed under the BSD 2-Clause License.
// See LICENSE for details.
//
//===------------------------------------------------------------------------===
#include "libmin.h"
#include "clause.h"
#include "solver.h"
#include "watch_list.h"
#include "mem.h"
#include "misc.h"

//===------------------------------------------------------------------------===
// Solver internal functions
//===------------------------------------------------------------------------===
static inline uint32_t
solver_decide(solver_t *s)
{
	uint32_t next_var = UNDEF;

	while (next_var == UNDEF || var_value(s, next_var) != VAR_UNASSING) {
		if (vec_size(s->var_order) == 0) {
			next_var = UNDEF;
			return UNDEF;
		}
		next_var = vec_at(s->var_order, 0);
		vec_drop(s->var_order, 0);
	}
	return var2lit(next_var, LIT_FALSE);
}

static inline void
solver_new_decision(solver_t *s, uint32_t lit)
{
	libmin_assert(var_value(s, lit2var(lit)) == VAR_UNASSING);
	vec_push_back(s->trail_lim, vec_size(s->trail));
	solver_enqueue(s, lit, UNDEF);
}

/** 
 *
 */
static inline void 
solver_backjump(solver_t *s, uint32_t level)
{
	if (solver_dlevel(s) <= level)
		return;
	for (uint32_t i = vec_size(s->trail); i-- > vec_at(s->trail_lim, level); ) {
		uint32_t var = lit2var(vec_at(s->trail, i));

		vec_assign(s->assigns, var, VAR_UNASSING);
		vec_assign(s->reasons, var, UNDEF);
		vec_push_back(s->var_order, var);
	}
	vec_sort(s->var_order, 1);
	s->i_qhead = vec_at(s->trail_lim, level);
	vec_shrink(s->trail, vec_at(s->trail_lim, level));
	vec_shrink(s->trail_lim, level);
}

/* Calculate Backtrack Level from the learnt clause */
static inline uint32_t
solver_calc_bt_level(solver_t *s, vec_ui32_t *learnt)
{
	uint32_t i, tmp;
        uint32_t i_max = 1;
	uint32_t *lits = vec_data(learnt);
        uint32_t max = lit_dlevel(s, lits[1]);

	if (vec_size(learnt) == 1)
		return 0;
	for (i = 2; i < vec_size(learnt); i++) {
		if (lit_dlevel(s, lits[i]) > max) {
			max   = lit_dlevel(s, lits[i]);
			i_max = i;
		}
	}
        tmp         = lits[1];
        lits[1]     = lits[i_max];
        lits[i_max] = tmp;
        return lit_dlevel(s, lits[1]);
}

/**
 *  Most books and papers explain conflict analysis and the calculation of the
 *  1UIP (first Unique Implication Point) using an implication graph. This
 *  function, however, do not explicity constructs the graph! It inspects the
 *  trail in reverse and figure out which literals should be added to the
 *  to-be-learnt clause using the reasons of each assignment.
 *
 *  cur_lit: current literal being analyzed.
 *  n_paths: number of unprocessed paths from conlfict node to the current
 *           literal being analyzed (cur_lit).
 *
 *  This functions performs a backward BFS (breadth-first search) for 1UIP node.
 *  The trail works as the BFS queue. The counter of unprocessed but seen
 *  variables (n_paths) allows us to identify when 'cur_lit' is the closest
 *  cause of conflict.
 *
 *  When 'n_paths' reaches zero it means there are no unprocessed reverse paths
 *  back from the conflict node to 'cur_lit' - meaning it is the 1UIP decision
 *  variable.
 *
 */
static inline void
solver_analyze(solver_t *s, uint32_t cref, vec_ui32_t *learnt, uint32_t *bt_level)
{
	uint32_t i;
	uint32_t *trail = vec_data(s->trail);
	uint32_t idx = vec_size(s->trail) - 1;
	uint32_t n_paths = 0;
	uint32_t p = UNDEF;
	uint32_t lit, var;

	vec_push_back(learnt, UNDEF);
	do {
		struct clause *clause;
		uint32_t *lits;
		uint32_t j;

		libmin_assert(cref != UNDEF);
		clause = clause_read(s, cref);
		lits = &(clause->lits[0]);

		if (p != UNDEF && clause->size == 2 && lit_value(s, lits[0]) == LIT_FALSE) {
			libmin_assert(lit_value(s, lits[1]) == LIT_TRUE);
			STM_SWAP(uint32_t, lits[0], lits[1] );
		}

		for (j = (p == UNDEF ? 0 : 1); j < clause->size; j++) {
			var = lit2var(lits[j]);
			if (vec_at(s->seen, var) || var_dlevel(s, var) == 0)
				continue;
			vec_assign(s->seen, var, 1);
			if (var_dlevel(s, var) == solver_dlevel(s)) {
				n_paths++;
			} else
				vec_push_back(learnt, lits[j]);
		}

		while (!vec_at(s->seen, lit2var(trail[idx--])));

		p = trail[idx + 1];
		cref = lit_reason(s, p);
		vec_assign(s->seen, lit2var(p), 0);
		n_paths--;
	} while (n_paths > 0);

	vec_data(learnt)[0] = lit_neg(p);
	*bt_level = solver_calc_bt_level(s, learnt);
	vec_ui32_foreach(learnt, lit, i)
		vec_assign(s->seen, lit2var(lit), 0);
}

//===------------------------------------------------------------------------===
// Functions used to create the implication graph using graphviz.
// This is based on CryptoMinisat
//===------------------------------------------------------------------------===
static inline void
solver_create_graph_mark_vars(solver_t *s)
{
	while (vec_size(s->stack)) {
		uint32_t var = lit2var(vec_pop_back(s->stack));
		
		if (var_reason(s, var) == UNDEF)
			continue;
		
		struct clause *c = clause_read(s, var_reason(s, var));
		uint32_t *lits = &(c->lits[0]); (void)lits;
		for (uint32_t i = 0; i < c->size; i++) {
			var = lit2var(c->lits[i]);
			if (var_dlevel(s, var) == 0)
				continue;
			if (vec_at(s->seen, var))
				continue;

			vec_push_back(s->stack, c->lits[i]);
			vec_assign(s->seen, var, 1);
		}
	}
}

static inline void
solver_create_graph_edges(solver_t *s)
{
	uint32_t i, lit;
	vec_ui32_foreach(s->trail, lit, i) {
		uint32_t var = lit2var(lit);

		if (var_dlevel(s, var) == 0)
			continue;
		if (vec_at(s->seen, var) == 0)
			continue;
		if (var_reason(s, var) == UNDEF)
			continue;

		struct clause *c = clause_read(s, var_reason(s, var));
		uint32_t *lits = &(c->lits[0]);
		for (uint32_t j = 0; j < c->size; j++) {
			if (lits[j] == lit || var_dlevel(s, lit2var(lits[j])) == 0)
				continue;

			libmin_printf("x%d -> x%d", lit2var(lits[j]), lit2var(lit));
			libmin_printf("[ label=\"");
			for (uint32_t k = 0; k < c->size; k++) {
				if (var_dlevel(s, lit2var(lits[k])) == 0)
					continue;
				libmin_printf("%s%d ", lit_polarity(lits[k]) ? "-" : "", lit2var(lits[k]));
			}
			libmin_printf("\", fontsize=8 ];\n");
		}
	}
}

static inline void
solver_create_graph_nodes(solver_t *s)
{
	uint32_t i, lit;
	vec_ui32_foreach(s->trail, lit, i) {
		uint32_t var = lit2var(lit);

		if (vec_at(s->seen, var) == 0)
			continue;
		vec_assign(s->seen, var, 0);

		libmin_printf("x%d [ shape=\"box\", style=\"filled\"", var);
		if (var_reason(s, var) == UNDEF)
			libmin_printf(", color=\"#b3cde3\"");
		else
			libmin_printf(", color=\"#ccebc5\"");

		libmin_printf(", label=\"%sx%d @ %d\" ];\n",  
		        (lit_polarity(lit) ? "-" : ""), var, var_dlevel(s, var));
	}
}

static inline void
solver_create_graph(solver_t *s, uint32_t cref, vec_ui32_t *learnt, uint32_t *bt_level)
{
	uint32_t i, lit;

	solver_analyze(s, cref, s->temp_lits, bt_level);

	libmin_printf("digraph G {\n");
	libmin_printf("vertK -> dummy [style=invis];\n");
	libmin_printf(" dummy [ shape=record, label=\"{");
	libmin_printf(" Learnt Clause: ");
	vec_ui32_foreach(learnt, lit, i) 
		libmin_printf("%s%d ", lit_polarity(lit) ? "-" : "", lit2var(lit));		
	libmin_printf("| Backtrack Level: %d", *bt_level);
	// fprintf(file, " | {resol: | " << res << " }");
	libmin_printf("}\"");
	libmin_printf(" , fontsize=8");
	libmin_printf(" ];\n");

	struct clause *clause = clause_read(s, cref);
	uint32_t *lits = &(clause->lits[0]);
	vec_clear(s->stack);
	for (uint32_t j = 0; j < clause->size; j++) {
		vec_assign(s->seen, lit2var(lits[j]), 1);
		vec_push_back(s->stack, lits[j]);
	}

	for (uint32_t j = 0; j < clause->size; j++) {
	        libmin_printf("x%d  -> vertK", lit2var(lits[j]));
		libmin_printf("[ label=\"");
		vec_ui32_foreach(s->stack, lit, i) 
			libmin_printf("%s%d ", lit_polarity(lit) ? "-" : "", lit2var(lit));
		libmin_printf("\", fontsize=8 ];\n");
	}

	/* Special conflict node */
	libmin_printf("vertK [ shape=\"box\", style=\"filled\", ");
	libmin_printf("color=\"#fbb4ae\", label=\"C : ");
	vec_ui32_foreach(s->stack, lit, i) 
		libmin_printf("%s%d ", lit_polarity(lit) ? "-" : "", lit2var(lit));
	libmin_printf("\"];\n");

	solver_create_graph_mark_vars(s);
	solver_create_graph_edges(s);
	solver_create_graph_nodes(s);

	libmin_printf("}\n");
}
//===------------------------------------------------------------------------===
// Solver external functions
//===------------------------------------------------------------------------===
/** 
 *  Creating a clause involves: calculating its size as a number of 32-bits
 *  words (n_words) and asking the clause database to make space for it. The
 *  database will return a reference, an index, to where the clause begins (cref) 
 *  with which we can obtain a clause handler to access and modify its info.
 *
 *  As we add literals to the clause, we must also add the clause to the
 *  ocurrence list of the literals. 
 */
uint32_t
solver_clause_create(solver_t *s, vec_ui32_t *lits)
{
	struct clause *clause;
	uint32_t cref;
	uint32_t n_words;

	libmin_assert(vec_size(lits) > 1);

	n_words = 1 + vec_size(lits);
	cref = cdb_append(s->clause_db, n_words);
	clause = clause_read(s, cref);
	clause->size = vec_size(lits);
	libmin_memcpy(&(clause->lits[0]), vec_data(lits), sizeof(uint32_t) * vec_size(lits));

	vec_push_back(s->clauses, cref);
	s->stats.n_lits += vec_size(lits);
	return cref;
}

uint32_t
solver_propagate(solver_t *s)
{
	uint32_t conf_cref = UNDEF;
	uint32_t n_propagations = 0;

	while (s->i_qhead < vec_size(s->trail)) {
		uint32_t lit = vec_at(s->trail, s->i_qhead++);
		uint32_t neg_lit;
		uint32_t *lits;
		struct watch_list *ws;
		struct watcher *begin;
		struct watcher *end;
		struct watcher *i, *j;

		n_propagations++;
		watch_list_foreach_bin(s->watches, i, lit) {
			if (var_value(s, lit2var(i->blocker)) == VAR_UNASSING)
				solver_enqueue(s, i->blocker, i->cref);
			else if (lit_value(s, i->blocker) == LIT_FALSE)
				return i->cref;
		}

		ws = vec_wl_at(s->watches, lit);
		begin = watch_list_array(ws);
		end = begin + watch_list_size(ws);
		for (i = j = begin + ws->n_bin; i < end;) {
			struct clause *clause;
			struct watcher w;

			if (lit_value(s, i->blocker) == LIT_TRUE) {
				*j++ = *i++;
				continue;
			}

			clause = clause_read(s, i->cref);
			lits = &(clause->lits[0]);

			// Make sure the false literal is data[1]:
			neg_lit = lit_neg(lit);
			if (lits[0] == neg_lit)
				STM_SWAP(uint32_t, lits[0], lits[1]);
			libmin_assert(lits[1] == neg_lit);

			w.cref = i->cref;
			w.blocker = lits[0];

			/* If 0th watch is true, then clause is already satisfied. */
			if (lits[0] != i->blocker && lit_value(s, lits[0]) == LIT_TRUE)
				*j++ = w;
			else {
				/* Look for new watch */
				for (uint32_t k = 2; k < clause->size; k++) {
					if (lit_value(s, lits[k]) != LIT_FALSE) {
						lits[1] = lits[k];
						lits[k] = neg_lit;
						watch_list_push(vec_wl_at(s->watches, lit_neg(lits[1])), w, 0);
						goto next;
					}
				}

				*j++ = w;

				/* Clause becomes unit under this assignment */
				if (lit_value(s, lits[0]) == LIT_FALSE) {
					conf_cref = i->cref;
					s->i_qhead = vec_size(s->trail);
					i++;
					// Copy the remaining watches:
					while (i < end)
						*j++ = *i++;
				} else
					solver_enqueue(s, lits[0], i->cref);
			}
		next:
			i++;
		}

		s->stats.n_inspects += j - watch_list_array(ws);
		watch_list_shrink(ws, j - watch_list_array(ws));
	}
	s->stats.n_propagations += n_propagations;
	return conf_cref;
}

/**
 *
 */
int
solver_search(solver_t *s)
{
	while (1) {
		uint32_t confl_cref = solver_propagate(s);
		uint32_t next_lit;
		if (confl_cref != UNDEF) {
			uint32_t bt_level;
			uint32_t cref = UNDEF;
			s->stats.n_conflicts++;
			if (solver_dlevel(s) == 0)
				return SATOMI_UNSAT;

			vec_clear(s->temp_lits);
			if (s->opts.verbose & 2)
				solver_create_graph(s, confl_cref, s->temp_lits, &bt_level);
			else
				solver_analyze(s, confl_cref, s->temp_lits, &bt_level);

			solver_backjump(s, bt_level);
			if (vec_size(s->temp_lits) > 1) {
				cref = solver_clause_create(s, s->temp_lits);
				clause_watch(s, cref);
			}
			solver_enqueue(s, vec_at(s->temp_lits, 0), cref);
		} else {
			s->stats.n_decisions++;
			next_lit = solver_decide(s);
			if (next_lit == UNDEF)
				return SATOMI_SAT;
			solver_new_decision(s, next_lit);
		}
	}
}

