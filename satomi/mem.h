//===--- mem.h --------------------------------------------------------------===
//
//                     satomi: Satisfiability solver
//
// This file is distributed under the BSD 2-Clause License.
// See LICENSE for details.
//
//===------------------------------------------------------------------------===
#ifndef SATOMI__UTILS__MEM_H
#define SATOMI__UTILS__MEM_H

#include "libmin.h"

#define STM_ALLOC(type, n) ((type *) libmin_malloc((n) * sizeof(type)))
#define STM_CALLOC(type, n) ((type *) libmin_calloc((n), sizeof(type)))
#define STM_REALLOC(type, ptr, n) ((type *) libmin_realloc(ptr, (n) * sizeof(type)))
#define STM_FREE(p) do { libmin_free(p); p = NULL; } while(0)

#endif /* SATOMI__UTILS__MEM_H */

