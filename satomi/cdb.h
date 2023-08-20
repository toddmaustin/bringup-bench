//===--- cdb.h --------------------------------------------------------------===
//
//                     satomi: Satisfiability solver
//
// This file is distributed under the BSD 2-Clause License.
// See LICENSE for details.
//
//===------------------------------------------------------------------------===
#ifndef SATOMI__CDB_H
#define SATOMI__CDB_H

#include "libmin.h"
#include "clause.h"
#include "mem.h"

/* Clauses DB */
struct cdb {
	uint32_t size;
	uint32_t cap;
	uint32_t wasted;
	uint32_t *data;
};

//===------------------------------------------------------------------------===
// Clause DB API
//===------------------------------------------------------------------------===
static inline struct clause *
cdb_handler(struct cdb *p, uint32_t cref)
{
	return cref != 0xFFFFFFFF ? (struct clause *)(p->data + cref) : NULL;
}

static inline uint32_t
cdb_cref(struct cdb *p, uint32_t *clause)
{
	return (uint32_t)(clause - &(p->data[0]));
}

static inline void
cdb_grow(struct cdb *p, uint32_t cap)
{
	uint32_t prev_cap = p->cap;

	if (p->cap >= cap)
		return;
	while (p->cap < cap) {
		uint32_t delta = ((p->cap >> 1) + (p->cap >> 3) + 2) & (uint32_t)(~1);
		p->cap += delta;
		libmin_assert(p->cap >= prev_cap);
	}
	libmin_assert(p->cap > 0);
	p->data = STM_REALLOC(uint32_t, p->data, p->cap);
}

static inline struct cdb *
cdb_alloc(uint32_t cap)
{
	struct cdb *p = STM_CALLOC(struct cdb, 1);
	if (cap <= 0)
		cap = 1024 * 1024;
	cdb_grow(p, cap);
	return p;
}

static inline void
cdb_free(struct cdb *p)
{
	STM_FREE(p->data);
	STM_FREE(p);
}

static inline uint32_t
cdb_append(struct cdb *p, uint32_t size)
{
	uint32_t prev_size;
	libmin_assert(size > 0);
	cdb_grow(p, p->size + size);
	prev_size = p->size;
	p->size += size;
	libmin_assert(p->size > prev_size);
	return prev_size;
}

static inline void
cdb_remove(struct cdb *p, struct clause *clause) { p->wasted += clause->size; }

static inline uint32_t
cdb_capacity(struct cdb *p) { return p->cap; }

static inline uint32_t
cdb_size(struct cdb *p) { return p->size; }

static inline uint32_t
cdb_wasted(struct cdb *p) { return p->wasted; }

#endif /* SATOMI__CDB_H */

