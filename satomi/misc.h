//===--- misc.h -------------------------------------------------------------===
//
//                     satomi: Satisfiability solver
//
// This file is distributed under the BSD 2-Clause License.
// See LICENSE for details.
//
//===------------------------------------------------------------------------===
#ifndef SATOMI__UTILS__MISC_H
#define SATOMI__UTILS__MISC_H

#include "libmin.h"

#define STM_SWAP(type, a, b)  { type t = a; a = b; b = t; }

static inline int
stm_ui32_comp_desc(const void *p1, const void *p2)
{
	const uint32_t pp1 = *(const uint32_t *)p1;
	const uint32_t pp2 = *(const uint32_t *)p2;

	if (pp1 > pp2)
		return -1;
	if (pp1 < pp2)
		return 1;
	return 0;
}

#endif /* SATOMI__UTILS__MISC_H */

