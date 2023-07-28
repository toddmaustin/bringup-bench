//===--- clause.h -----------------------------------------------------------===
//
//                     satomi: Satisfiability solver
//
// This file is distributed under the BSD 2-Clause License.
// See LICENSE for details.
//
//===------------------------------------------------------------------------===
#ifndef SATOMI__CLAUSE_H
#define SATOMI__CLAUSE_H

#include <stdint.h>
#include <stdio.h>

struct clause {
	uint32_t size;
	uint32_t lits[0];
};

//===------------------------------------------------------------------------===
// Clause API
//===------------------------------------------------------------------------===
static inline void
clause_print(struct clause *clause)
{
	fprintf(stdout, "{ ");
	for (uint32_t i = 0; i < clause->size; i++)
		fprintf(stdout, "%u ", clause->lits[i]);
	fprintf(stdout, "}\n");
}

#endif /* SATOMI__CLAUSE_H */
