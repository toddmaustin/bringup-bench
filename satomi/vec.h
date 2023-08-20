//===--- vec.h --------------------------------------------------------------===
//
//                     satomi: Satisfiability solver
//
// This file is distributed under the BSD 2-Clause License.
// See LICENSE for details.
//
//===------------------------------------------------------------------------===
#ifndef SATOMI__UTILS__VEC__VEC_H
#define SATOMI__UTILS__VEC__VEC_H

//===--- vec_i32.h ---------------------------------------------------------===
//
//                     satomi: Satisfiability solver
//
// This file is distributed under the BSD 2-Clause License.
// See LICENSE for details.
//
//===------------------------------------------------------------------------===

#include "libmin.h"
#include "mem.h"

typedef struct vec_i32_t_ vec_i32_t;
struct vec_i32_t_ {
	uint32_t cap;
	uint32_t size;
	int32_t* data;
};

//===------------------------------------------------------------------------===
// Vector Macros
//===------------------------------------------------------------------------===
#define vec_i32_foreach(vec, entry, i) \
    for (i = 0; (i < vec_i32_size(vec)) && (((entry) = vec_i32_at(vec, i)), 1); i++)

#define vec_i32_foreach_start(vec, entry, i, start) \
    for (i = start; (i < vec_i32_size(vec)) && (((entry) = vec_i32_at(vec, i)), 1); i++)

#define vec_i32_foreach_stop(vec, entry, i, stop) \
    for (i = 0; (i < stop) && (((entry) = vec_i32_at(vec, i)), 1); i++)

//===------------------------------------------------------------------------===
// Vector API
//===------------------------------------------------------------------------===
static inline vec_i32_t *
vec_i32_alloc(uint32_t cap)
{
	vec_i32_t *vec = STM_ALLOC(vec_i32_t, 1);

	if (cap > 0 && cap < 16)
		cap = 16;
	vec->size = 0;
	vec->cap = cap;
	vec->data = vec->cap ? STM_ALLOC(int32_t, vec->cap) : NULL;
	return vec;
}

static inline vec_i32_t *
vec_i32_alloc_exact(uint32_t cap)
{
	vec_i32_t *vec = STM_ALLOC(vec_i32_t, 1);

	cap = 0;
	vec->size = 0;
	vec->cap = cap;
	vec->data = vec->cap ? STM_ALLOC(int32_t, vec->cap ) : NULL;
	return vec;
}

static inline vec_i32_t *
vec_i32_init(uint32_t size, int32_t value)
{
	vec_i32_t *vec = STM_ALLOC(vec_i32_t, 1);

	vec->cap = size;
	vec->size = size;
	vec->data = vec->cap ? STM_ALLOC(int32_t, vec->cap) : NULL;
	libmin_memset(vec->data, value, sizeof(int32_t) * vec->size);
	return vec;
}

static inline void
vec_i32_free(vec_i32_t *vec)
{
	if (vec->data != NULL)
		STM_FREE(vec->data);
	STM_FREE(vec);
}

static inline uint32_t
vec_i32_size(vec_i32_t *vec)
{
	return vec->size;
}

static inline void
vec_i32_resize(vec_i32_t *vec, uint32_t new_size)
{
	vec->size = new_size;
	if (vec->cap >= new_size)
		return;
	vec->data = STM_REALLOC(int32_t, vec->data, new_size);
	libmin_assert(vec->data != NULL);
	vec->cap = new_size;
}

static inline void
vec_i32_shrink(vec_i32_t *vec, uint32_t new_size)
{
	libmin_assert(vec->cap >= new_size);
	vec->size = new_size;
}

static inline void
vec_i32_reserve(vec_i32_t *vec, uint32_t new_cap)
{
	if (vec->cap >= new_cap)
		return;
	vec->data = STM_REALLOC(int32_t, vec->data, new_cap);
	libmin_assert(vec->data != NULL);
	vec->cap = new_cap;
}

static inline uint32_t
vec_i32_capacity(vec_i32_t *vec)
{
	return vec->cap;
}

static inline int
vec_i32_empty(vec_i32_t *vec)
{
	return vec->size ? 0 : 1;
}

static inline void
vec_i32_erase(vec_i32_t *vec)
{
	STM_FREE(vec->data);
	vec->size = 0;
	vec->cap = 0;
}

static inline int32_t
vec_i32_at(vec_i32_t *vec, uint32_t idx)
{
	libmin_assert(idx >= 0 && idx < vec->size);
	return vec->data[idx];
}

static inline int32_t *
vec_i32_at_ptr(vec_i32_t *vec, uint32_t idx)
{
	libmin_assert(idx >= 0 && idx < vec->size);
	return vec->data + idx;
}

static inline int32_t
vec_i32_find(vec_i32_t *vec, int32_t entry)
{
	for (uint32_t i = 0; i < vec->size; i++)
		if (vec->data[i] == entry)
			return 1;
	return 0;
}

static inline int32_t *
vec_i32_data(vec_i32_t *vec)
{
	libmin_assert(vec);
	return vec->data;
}

static inline void
vec_i32_duplicate(vec_i32_t *dest, const vec_i32_t *src)
{
	libmin_assert(dest != NULL && src != NULL);
	vec_i32_resize(dest, src->cap);
	libmin_memcpy(dest->data, src->data, sizeof(int32_t) * src->cap);
	dest->size = src->size;
}

static inline void
vec_i32_copy(vec_i32_t *dest, const vec_i32_t *src)
{
	libmin_assert(dest != NULL && src != NULL);
	vec_i32_resize(dest, src->size);
	libmin_memcpy(dest->data, src->data, sizeof(int32_t) * src->size);
	dest->size = src->size;
}

static inline void
vec_i32_push_back(vec_i32_t *vec, int32_t value)
{
	if (vec->size == vec->cap) {
		if (vec->cap < 16)
			vec_i32_reserve(vec, 16);
		else
			vec_i32_reserve(vec, 2 * vec->cap);
	}
	vec->data[vec->size] = value;
	vec->size++;
}

static inline int32_t
vec_i32_pop_back(vec_i32_t *vec)
{
	libmin_assert(vec && vec->size);
	return vec->data[--vec->size];
}

static inline void
vec_i32_assign(vec_i32_t *vec, uint32_t idx, int32_t value)
{
	libmin_assert((idx >= 0) && (idx < vec_i32_size(vec)));
	vec->data[idx] = value;
}

static inline void
vec_i32_insert(vec_i32_t *vec, uint32_t idx, int32_t value)
{
	libmin_assert((idx >= 0) && (idx < vec_i32_size(vec)));
	vec_i32_push_back(vec, 0);
	libmin_memmove(vec->data + idx + 1, vec->data + idx, (vec->size - idx - 2) * sizeof(int32_t));
	vec->data[idx] = value;
}

static inline void
vec_i32_drop(vec_i32_t *vec, uint32_t idx)
{
	libmin_assert((idx >= 0) && (idx < vec_i32_size(vec)));
	libmin_memmove(vec->data + idx, vec->data + idx + 1, (vec->size - idx - 1) * sizeof(int32_t));
	vec->size -= 1;
}

static inline void
vec_i32_clear(vec_i32_t *vec)
{
	vec->size = 0;
}

static inline int
vec_i32_asc_compare(const void *p1, const void *p2)
{
	const int32_t *pp1 = (const int32_t *) p1;
	const int32_t *pp2 = (const int32_t *) p2;

	if ( *pp1 < *pp2 )
		return -1;
	if ( *pp1 > *pp2 )
		return 1;
	return 0;
}

static inline int
vec_i32_desc_compare(const void *p1, const void *p2)
{
	const int32_t *pp1 = (const int32_t *) p1;
	const int32_t *pp2 = (const int32_t *) p2;

	if ( *pp1 > *pp2 )
		return -1;
	if ( *pp1 < *pp2 )
		return 1;
	return 0;
}

static inline void
vec_i32_sort(vec_i32_t *vec, int ascending)
{
	if (ascending)
		libmin_qsort((void *) vec->data, vec->size, sizeof(int32_t),
		      (int (*)(const void *, const void *)) vec_i32_asc_compare);
	else
		libmin_qsort((void*) vec->data, vec->size, sizeof(int32_t),
		      (int (*)(const void *, const void *)) vec_i32_desc_compare);
}

static inline uint32_t
vec_i32_memory(vec_i32_t *vec)
{
	return vec == NULL ? 0 : sizeof(int32_t) * vec->cap + sizeof(vec_i32_t);
}

static inline void
vec_i32_print(vec_i32_t* vec)
{
	libmin_assert(vec != NULL);
	libmin_printf("Vector has %u(%u) entries: {", vec->size, vec->cap);
	for (uint32_t i = 0; i < vec->size; i++)
		libmin_printf(" %d", vec->data[i]);
	libmin_printf(" }\n");
}

//===--- vec_ui8.h ----------------------------------------------------------===
//
//                     satomi: Satisfiability solver
//
// This file is distributed under the BSD 2-Clause License.
// See LICENSE for details.
//
//===------------------------------------------------------------------------===

#include "mem.h"

typedef struct vec_ui8_t_ vec_ui8_t;
struct vec_ui8_t_ {
	uint32_t cap;
	uint32_t size;
	uint8_t* data;
};

//===------------------------------------------------------------------------===
// Vector Macros
//===------------------------------------------------------------------------===
#define vec_ui8_foreach(vec, entry, i) \
    for (i = 0; (i < vec_ui8_size(vec)) && (((entry) = vec_ui8_at(vec, i)), 1); i++)

#define vec_ui8_foreach_start(vec, entry, i, start) \
    for (i = start; (i < vec_ui8_size(vec)) && (((entry) = vec_ui8_at(vec, i)), 1); i++)

#define vec_ui8_foreach_stop(vec, entry, i, stop) \
    for (i = 0; (i < stop) && (((entry) = vec_ui8_at(vec, i)), 1); i++)

//===------------------------------------------------------------------------===
// Vector API
//===------------------------------------------------------------------------===
static inline vec_ui8_t *
vec_ui8_alloc(uint32_t cap)
{
	vec_ui8_t *vec = STM_ALLOC(vec_ui8_t, 1);

	if (cap > 0 && cap < 16)
		cap = 16;
	vec->size = 0;
	vec->cap = cap;
	vec->data = vec->cap ? STM_ALLOC(uint8_t, vec->cap) : NULL;
	return vec;
}

static inline vec_ui8_t *
vec_ui8_alloc_exact(uint32_t cap)
{
	vec_ui8_t *vec = STM_ALLOC(vec_ui8_t, 1);

	cap = 0;
	vec->size = 0;
	vec->cap = cap;
	vec->data = vec->cap ? STM_ALLOC(uint8_t, vec->cap ) : NULL;
	return vec;
}

static inline vec_ui8_t *
vec_ui8_init(uint32_t size, uint8_t value)
{
	vec_ui8_t *vec = STM_ALLOC(vec_ui8_t, 1);

	vec->cap = size;
	vec->size = size;
	vec->data = vec->cap ? STM_ALLOC(uint8_t, vec->cap) : NULL;
	libmin_memset(vec->data, value, sizeof(uint8_t) * vec->size);
	return vec;
}

static inline void
vec_ui8_free(vec_ui8_t *vec)
{
	if (vec->data != NULL)
		STM_FREE(vec->data);
	STM_FREE(vec);
}

static inline uint32_t
vec_ui8_size(vec_ui8_t *vec) { return vec->size; }

static inline void
vec_ui8_resize(vec_ui8_t *vec, uint32_t new_size)
{
	vec->size = new_size;
	if (vec->cap >= new_size)
		return;
	vec->data = STM_REALLOC(uint8_t, vec->data, new_size);
	libmin_assert(vec->data != NULL);
	vec->cap = new_size;
}

static inline void
vec_ui8_shrink(vec_ui8_t *vec, uint32_t new_size)
{
	libmin_assert(vec->cap >= new_size);
	vec->size = new_size;
}

static inline void
vec_ui8_reserve(vec_ui8_t *vec, uint32_t new_cap)
{
	if (vec->cap >= new_cap)
		return;
	vec->data = STM_REALLOC(uint8_t, vec->data, new_cap);
	libmin_assert(vec->data != NULL);
	vec->cap = new_cap;
}

static inline uint32_t
vec_ui8_capacity(vec_ui8_t *vec) { return vec->cap; }

static inline int
vec_ui8_empty(vec_ui8_t *vec) { return vec->size ? 0 : 1; }

static inline void
vec_ui8_erase(vec_ui8_t *vec)
{
	STM_FREE(vec->data);
	vec->size = 0;
	vec->cap = 0;
}

static inline uint8_t
vec_ui8_at(vec_ui8_t *vec, uint32_t idx)
{
	libmin_assert(idx >= 0 && idx < vec->size);
	return vec->data[idx];
}

static inline uint8_t *
vec_ui8_at_ptr(vec_ui8_t *vec, uint32_t idx)
{
	libmin_assert(idx >= 0 && idx < vec->size);
	return vec->data + idx;
}

static inline uint8_t
vec_ui8_find(vec_ui8_t *vec, uint8_t entry)
{
	for (uint32_t i = 0; i < vec->size; i++)
		if (vec->data[i] == entry)
			return 1;
	return 0;
}

static inline uint8_t *
vec_ui8_data(vec_ui8_t *vec)
{
	libmin_assert(vec);
	return vec->data;
}

static inline void
vec_ui8_duplicate(vec_ui8_t *dest, const vec_ui8_t *src)
{
	libmin_assert(dest != NULL && src != NULL);
	vec_ui8_resize(dest, src->cap);
	libmin_memcpy(dest->data, src->data, sizeof(uint8_t) * src->cap);
	dest->size = src->size;
}

static inline void
vec_ui8_copy(vec_ui8_t *dest, const vec_ui8_t *src)
{
	libmin_assert(dest != NULL && src != NULL);
	vec_ui8_resize(dest, src->size);
	libmin_memcpy(dest->data, src->data, sizeof(uint8_t) * src->size);
	dest->size = src->size;
}

static inline void
vec_ui8_push_back(vec_ui8_t *vec, uint8_t value)
{
	if (vec->size == vec->cap) {
		if (vec->cap < 16)
			vec_ui8_reserve(vec, 16);
		else
			vec_ui8_reserve(vec, 2 * vec->cap);
	}
	vec->data[vec->size] = value;
	vec->size++;
}

static inline uint8_t
vec_ui8_pop_back(vec_ui8_t *vec)
{
	libmin_assert(vec && vec->size);
	return vec->data[--vec->size];
}

static inline void
vec_ui8_assign(vec_ui8_t *vec, uint32_t idx, uint8_t value)
{
	libmin_assert((idx >= 0) && (idx < vec_ui8_size(vec)));
	vec->data[idx] = value;
}

static inline void
vec_ui8_insert(vec_ui8_t *vec, uint32_t idx, uint8_t value)
{
	libmin_assert((idx >= 0) && (idx < vec_ui8_size(vec)));
	vec_ui8_push_back(vec, 0);
	libmin_memmove(vec->data + idx + 1, vec->data + idx, (vec->size - idx - 2) * sizeof(uint8_t));
	vec->data[idx] = value;
}

static inline void
vec_ui8_drop(vec_ui8_t *vec, uint32_t idx)
{
	libmin_assert((idx >= 0) && (idx < vec_ui8_size(vec)));
	libmin_memmove(vec->data + idx, vec->data + idx + 1, (vec->size - idx - 1) * sizeof(uint8_t));
	vec->size -= 1;
}

static inline void
vec_ui8_clear(vec_ui8_t *vec)
{
	vec->size = 0;
}

static inline int
vec_ui8_asc_compare(const void *p1, const void *p2)
{
	const uint8_t *pp1 = (const uint8_t *) p1;
	const uint8_t *pp2 = (const uint8_t *) p2;

	if ( *pp1 < *pp2 )
		return -1;
	if ( *pp1 > *pp2 )
		return 1;
	return 0;
}

static inline int
vec_ui8_desc_compare(const void *p1, const void *p2)
{
	const uint8_t *pp1 = (const uint8_t *) p1;
	const uint8_t *pp2 = (const uint8_t *) p2;

	if ( *pp1 > *pp2 )
		return -1;
	if ( *pp1 < *pp2 )
		return 1;
	return 0;
}

static inline void
vec_ui8_sort(vec_ui8_t *vec, int ascending)
{
	if (ascending)
		libmin_qsort((void *) vec->data, vec->size, sizeof(uint8_t),
		      (int (*)(const void *, const void *)) vec_ui8_asc_compare);
	else
		libmin_qsort((void*) vec->data, vec->size, sizeof(uint8_t),
		      (int (*)(const void *, const void *)) vec_ui8_desc_compare);
}

static inline uint32_t
vec_ui8_memory(vec_ui8_t *vec)
{
	return vec == NULL ? 0 : sizeof(uint8_t) * vec->cap + sizeof(vec_ui8_t);
}

static inline void
vec_ui8_print(vec_ui8_t* vec)
{
	libmin_assert(vec != NULL);
	libmin_printf("Vector has %u(%u) entries: {", vec->size, vec->cap);
	for (uint32_t i = 0; i < vec->size; i++)
		libmin_printf(" %u", vec->data[i]);
	libmin_printf(" }\n");
}

//===--- vec_ui32.h ---------------------------------------------------------===
//
//                     satomi: Satisfiability solver
//
// This file is distributed under the BSD 2-Clause License.
// See LICENSE for details.
//
//===------------------------------------------------------------------------===

#include "mem.h"

typedef struct vec_ui32_t_ vec_ui32_t;
struct vec_ui32_t_ {
	uint32_t cap;
	uint32_t size;
	uint32_t* data;
};

//===------------------------------------------------------------------------===
// Vector Macros
//===------------------------------------------------------------------------===
#define vec_ui32_foreach(vec, entry, i) \
    for (i = 0; (i < vec_ui32_size(vec)) && (((entry) = vec_ui32_at(vec, i)), 1); i++)

#define vec_ui32_foreach_start(vec, entry, i, start) \
    for (i = start; (i < vec_ui32_size(vec)) && (((entry) = vec_ui32_at(vec, i)), 1); i++)

#define vec_ui32_foreach_stop(vec, entry, i, stop) \
    for (i = 0; (i < stop) && (((entry) = vec_ui32_at(vec, i)), 1); i++)

//===------------------------------------------------------------------------===
// Vector API
//===------------------------------------------------------------------------===
static inline vec_ui32_t *
vec_ui32_alloc(uint32_t cap)
{
	vec_ui32_t *vec = STM_ALLOC(vec_ui32_t, 1);

	if (cap > 0 && cap < 16)
		cap = 16;
	vec->size = 0;
	vec->cap = cap;
	vec->data = vec->cap ? STM_ALLOC(uint32_t, vec->cap) : NULL;
	return vec;
}

static inline vec_ui32_t *
vec_ui32_alloc_exact(uint32_t cap)
{
	vec_ui32_t *vec = STM_ALLOC(vec_ui32_t, 1);

	cap = 0;
	vec->size = 0;
	vec->cap = cap;
	vec->data = vec->cap ? STM_ALLOC(uint32_t, vec->cap ) : NULL;
	return vec;
}

static inline vec_ui32_t *
vec_ui32_init(uint32_t size, uint32_t value)
{
	vec_ui32_t *vec = STM_ALLOC(vec_ui32_t, 1);

	vec->cap = size;
	vec->size = size;
	vec->data = vec->cap ? STM_ALLOC(uint32_t, vec->cap) : NULL;
	libmin_memset(vec->data, value, sizeof(uint32_t) * vec->size);
	return vec;
}

static inline void
vec_ui32_free(vec_ui32_t *vec)
{
	if (vec->data != NULL)
		STM_FREE(vec->data);
	STM_FREE(vec);
}

static inline uint32_t
vec_ui32_size(vec_ui32_t *vec) { return vec->size; }

static inline void
vec_ui32_resize(vec_ui32_t *vec, uint32_t new_size)
{
	vec->size = new_size;
	if (vec->cap >= new_size)
		return;
	vec->data = STM_REALLOC(uint32_t, vec->data, new_size);
	libmin_assert(vec->data != NULL);
	vec->cap = new_size;
}

static inline void
vec_ui32_shrink(vec_ui32_t *vec, uint32_t new_size)
{
	libmin_assert(vec->cap >= new_size);
	vec->size = new_size;
}

static inline void
vec_ui32_reserve(vec_ui32_t *vec, uint32_t new_cap)
{
	if (vec->cap >= new_cap)
		return;
	vec->data = STM_REALLOC(uint32_t, vec->data, new_cap);
	libmin_assert(vec->data != NULL);
	vec->cap = new_cap;
}

static inline uint32_t
vec_ui32_capacity(vec_ui32_t *vec) { return vec->cap; }

static inline int
vec_ui32_empty(vec_ui32_t *vec) { return vec->size ? 0 : 1; }

static inline void
vec_ui32_erase(vec_ui32_t *vec)
{
	STM_FREE(vec->data);
	vec->size = 0;
	vec->cap = 0;
}

static inline uint32_t
vec_ui32_at(vec_ui32_t *vec, uint32_t idx)
{
	libmin_assert(idx >= 0 && idx < vec->size);
	return vec->data[idx];
}

static inline uint32_t *
vec_ui32_at_ptr(vec_ui32_t *vec, uint32_t idx)
{
	libmin_assert(idx >= 0 && idx < vec->size);
	return vec->data + idx;
}

static inline uint32_t
vec_ui32_find(vec_ui32_t *vec, uint32_t entry)
{
	for (uint32_t i = 0; i < vec->size; i++)
		if (vec->data[i] == entry)
			return i;
	return UINT32_MAX;
}

static inline uint32_t *
vec_ui32_data(vec_ui32_t *vec)
{
	libmin_assert(vec);
	return vec->data;
}

static inline void
vec_ui32_duplicate(vec_ui32_t *dest, const vec_ui32_t *src)
{
	libmin_assert(dest != NULL && src != NULL);
	vec_ui32_resize(dest, src->cap);
	libmin_memcpy(dest->data, src->data, sizeof(uint32_t) * src->cap);
	dest->size = src->size;
}

static inline void
vec_ui32_copy(vec_ui32_t *dest, const vec_ui32_t *src)
{
	libmin_assert(dest != NULL && src != NULL);
	vec_ui32_resize(dest, src->size);
	libmin_memcpy(dest->data, src->data, sizeof(uint32_t) * src->size);
	dest->size = src->size;
}

static inline void
vec_ui32_push_back(vec_ui32_t *vec, uint32_t value)
{
	if (vec->size == vec->cap) {
		if (vec->cap < 16)
			vec_ui32_reserve(vec, 16);
		else
			vec_ui32_reserve(vec, 2 * vec->cap);
	}
	vec->data[vec->size] = value;
	vec->size++;
}

static inline uint32_t
vec_ui32_pop_back(vec_ui32_t *vec)
{
	libmin_assert(vec && vec->size);
	return vec->data[--vec->size];
}

static inline void
vec_ui32_assign(vec_ui32_t *vec, uint32_t idx, uint32_t value)
{
	libmin_assert((idx >= 0) && (idx < vec_ui32_size(vec)));
	vec->data[idx] = value;
}

static inline void
vec_ui32_insert(vec_ui32_t *vec, uint32_t idx, uint32_t value)
{
	libmin_assert((idx >= 0) && (idx < vec_ui32_size(vec)));
	vec_ui32_push_back(vec, 0);
	libmin_memmove(vec->data + idx + 1, vec->data + idx, (vec->size - idx - 2) * sizeof(uint32_t));
	vec->data[idx] = value;
}

static inline void
vec_ui32_drop(vec_ui32_t *vec, uint32_t idx)
{
	libmin_assert((idx >= 0) && (idx < vec_ui32_size(vec)));
	libmin_memmove(vec->data + idx, vec->data + idx + 1, (vec->size - idx - 1) * sizeof(uint32_t));
	vec->size -= 1;
}

static inline void
vec_ui32_clear(vec_ui32_t *vec)
{
	vec->size = 0;
}

static inline int
vec_ui32_asc_compare(const void *p1, const void *p2)
{
	const uint32_t *pp1 = (const uint32_t *) p1;
	const uint32_t *pp2 = (const uint32_t *) p2;

	if ( *pp1 < *pp2 )
		return -1;
	if ( *pp1 > *pp2 )
		return 1;
	return 0;
}

static inline int
vec_ui32_desc_compare(const void *p1, const void *p2)
{
	const uint32_t *pp1 = (const uint32_t *) p1;
	const uint32_t *pp2 = (const uint32_t *) p2;

	if ( *pp1 > *pp2 )
		return -1;
	if ( *pp1 < *pp2 )
		return 1;
	return 0;
}

static inline void
vec_ui32_sort(vec_ui32_t *vec, int ascending)
{
	if (ascending)
		libmin_qsort((void *) vec->data, vec->size, sizeof(uint32_t),
		      (int (*)(const void *, const void *)) vec_ui32_asc_compare);
	else
		libmin_qsort((void*) vec->data, vec->size, sizeof(uint32_t),
		      (int (*)(const void *, const void *)) vec_ui32_desc_compare);
}

static inline uint32_t
vec_ui32_memory(vec_ui32_t *vec)
{
	return vec == NULL ? 0 : sizeof(uint32_t) * vec->cap + sizeof(vec_ui32_t);
}

static inline void
vec_ui32_print(vec_ui32_t* vec)
{
	libmin_assert(vec != NULL);
	libmin_printf("Vector has %u(%u) entries: {", vec->size, vec->cap);
	for (uint32_t i = 0; i < vec->size; i++)
		libmin_printf(" %u", vec->data[i]);
	libmin_printf(" }\n");
}

#define vec_free(vec) _Generic((vec), \
	vec_i32_t *: vec_i32_free,    \
	vec_ui8_t *: vec_ui8_free,    \
	vec_ui32_t *: vec_ui32_free   \
	)(vec)

#define vec_size(vec) _Generic((vec), \
	vec_i32_t *: vec_i32_size,    \
	vec_ui8_t *: vec_ui8_size,    \
	vec_ui32_t *: vec_ui32_size   \
	)(vec)

#define vec_resize(vec, new_size) _Generic((vec), \
	vec_i32_t *: vec_i32_resize,    \
	vec_ui8_t *: vec_ui8_resize,    \
	vec_ui32_t *: vec_ui32_resize   \
	)(vec, new_size)

#define vec_shrink(vec, new_size) _Generic((vec), \
	vec_i32_t *: vec_i32_shrink,    \
	vec_ui8_t *: vec_ui8_shrink,    \
	vec_ui32_t *: vec_ui32_shrink   \
	)(vec, new_size)

#define vec_reserve(vec, new_cap) _Generic((vec), \
	vec_i32_t *: vec_i32_reserve,    \
	vec_ui8_t *: vec_ui8_reserve,    \
	vec_ui32_t *: vec_ui32_reserve   \
	)(vec, new_cap)

#define vec_capacity(vec) _Generic((vec), \
	vec_i32_t *: vec_i32_capacity,    \
	vec_ui8_t *: vec_ui8_capacity,    \
	vec_ui32_t *: vec_ui32_capacity   \
	)(vec)

#define vec_empty(vec) _Generic((vec), \
	vec_i32_t *: vec_i32_empty,    \
	vec_ui8_t *: vec_ui8_empty,    \
	vec_ui32_t *: vec_ui32_empty   \
	)(vec)

#define vec_erase(vec) _Generic((vec), \
	vec_i32_t *: vec_i32_erase,    \
	vec_ui8_t *: vec_ui8_erase,    \
	vec_ui32_t *: vec_ui32_erase   \
	)(vec)

#define vec_at(vec, idx) _Generic((vec), \
	vec_i32_t *: vec_i32_at,    \
	vec_ui8_t *: vec_ui8_at,    \
	vec_ui32_t *: vec_ui32_at   \
	)(vec, idx)

#define vec_at_ptr(vec, idx) _Generic((vec), \
	vec_i32_t *: vec_i32_at_ptr,    \
	vec_ui8_t *: vec_ui8_at_ptr,    \
	vec_ui32_t *: vec_ui32_at_ptr   \
	)(vec, idx)

#define vec_data(vec) _Generic((vec), \
	vec_i32_t *: vec_i32_data,    \
	vec_ui8_t *: vec_ui8_data,    \
	vec_ui32_t *: vec_ui32_data   \
	)(vec)

#define vec_find(vec, value) _Generic((vec), \
	vec_i32_t *: vec_i32_find,    \
	vec_ui8_t *: vec_ui8_find,    \
	vec_ui32_t *: vec_ui32_find   \
	)(vec, value)

#define vec_duplicate(dest, src) _Generic((dest), \
	vec_i32_t *: vec_i32_duplicate,    \
	vec_ui8_t *: vec_ui8_duplicate,    \
	vec_ui32_t *: vec_ui32_duplicate   \
	)(dest, src)

#define vec_copy(dest, src) _Generic((dest), \
	vec_i32_t *: vec_i32_copy,    \
	vec_ui8_t *: vec_ui8_copy,    \
	vec_ui32_t *: vec_ui32_copy   \
	)(dest, src)

#define vec_push_back(vec, value) _Generic((vec), \
	vec_i32_t *: vec_i32_push_back,    \
	vec_ui8_t *: vec_ui8_push_back,    \
	vec_ui32_t *: vec_ui32_push_back   \
	)(vec, value)

#define vec_pop_back(vec) _Generic((vec), \
	vec_i32_t *: vec_i32_pop_back,    \
	vec_ui8_t *: vec_ui8_pop_back,    \
	vec_ui32_t *: vec_ui32_pop_back   \
	)(vec)

#define vec_assign(vec, idx, value) _Generic((vec), \
	vec_i32_t *: vec_i32_assign,    \
	vec_ui8_t *: vec_ui8_assign,    \
	vec_ui32_t *: vec_ui32_assign   \
	)(vec, idx, value)

#define vec_insert(vec, idx, value) _Generic((vec), \
	vec_i32_t *: vec_i32_insert,    \
	vec_ui8_t *: vec_ui8_insert,    \
	vec_ui32_t *: vec_ui32_insert   \
	)(vec, idx, value)

#define vec_drop(vec, idx) _Generic((vec), \
	vec_i32_t *: vec_i32_drop,    \
	vec_ui8_t *: vec_ui8_drop,    \
	vec_ui32_t *: vec_ui32_drop   \
	)(vec, idx)

#define vec_clear(vec) _Generic((vec), \
	vec_i32_t *: vec_i32_clear,    \
	vec_ui8_t *: vec_ui8_clear,    \
	vec_ui32_t *: vec_ui32_clear   \
	)(vec)

#define vec_sort(vec, ascending) _Generic((vec), \
	vec_i32_t *: vec_i32_sort,    \
	vec_ui8_t *: vec_ui8_sort,    \
	vec_ui32_t *: vec_ui32_sort   \
	)(vec, ascending)

#define vec_memory(vec) _Generic((vec), \
	vec_i32_t *: vec_i32_memory,    \
	vec_ui8_t *: vec_ui8_memory,    \
	vec_ui32_t *: vec_ui32_memory   \
	)(vec)

#define vec_print(vec) _Generic((vec), \
	vec_i32_t *: vec_i32_print,    \
	vec_ui8_t *: vec_ui8_print,    \
	vec_ui32_t *: vec_ui32_print   \
	)(vec)

#endif /* SATOMI__UTILS__VEC__VEC_H */

