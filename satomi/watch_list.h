//===--- watch_list.h -------------------------------------------------------===
//
//                     satomi: Satisfiability solver
//
// This file is distributed under the BSD 2-Clause License.
// See LICENSE for details.
//
//===------------------------------------------------------------------------===
#ifndef SATOMI__WATCH_LIST_H
#define SATOMI__WATCH_LIST_H

#include "libmin.h"
#include "mem.h"
#include "misc.h"

struct watcher {
	uint32_t cref;
	uint32_t blocker;
};

struct watch_list {
	uint32_t cap;
	uint32_t size;
	uint32_t n_bin;
	struct watcher *watchers;
};

typedef struct vec_wl_t_ vec_wl_t;
struct vec_wl_t_ {
	uint32_t cap;
	uint32_t size;
	struct watch_list *watch_lists;
};

//===------------------------------------------------------------------------===
// Watch list Macros
//===------------------------------------------------------------------------===
#define watch_list_foreach(vec, watch, lit) \
    for (watch = watch_list_array(vec_wl_at(vec, lit)); \
         watch < watch_list_array(vec_wl_at(vec, lit)) + watch_list_size(vec_wl_at(vec, lit)); \
         watch++)

#define watch_list_foreach_bin(vec, watch, lit) \
    for (watch = watch_list_array(vec_wl_at(vec, lit)); \
         watch < watch_list_array(vec_wl_at(vec, lit)) + vec_wl_at(vec, lit)->n_bin; \
         watch++)
//===------------------------------------------------------------------------===
// Watch list API
//===------------------------------------------------------------------------===
static inline void
watch_list_free(struct watch_list *wl)
{
	if (wl->watchers)
		STM_FREE(wl->watchers);
}

static inline uint32_t
watch_list_size(struct watch_list *wl) { return wl->size; }

static inline void 
watch_list_shrink(struct watch_list *wl, uint32_t size)
{
	libmin_assert(size <= wl->size);
	wl->size = size;
}

static inline void
watch_list_push(struct watch_list *wl, struct watcher w, uint32_t is_bin)
{
	libmin_assert(wl);
	if (wl->size == wl->cap) {
		uint32_t new_size = (wl->cap < 4) ? 4 : (wl->cap / 2) * 3;
		struct watcher *watchers =
		    STM_REALLOC(struct watcher, wl->watchers, new_size);
		if (watchers == NULL) {
			libmin_printf("Failed to realloc memory from %.1f MB to %.1f "
			       "MB.\n",
			       1.0 * wl->cap / (1 << 20),
			       1.0 * new_size / (1 << 20));
			return;
		}
		wl->watchers = watchers;
		wl->cap = new_size;
	}
	wl->watchers[wl->size++] = w;
	if (is_bin && wl->size > wl->n_bin) {
		STM_SWAP(struct watcher, wl->watchers[wl->n_bin], wl->watchers[wl->size - 1]);
		wl->n_bin++;
	}
}

static inline struct watcher *
watch_list_array(struct watch_list *wl)
{
	return wl->watchers;
}

static inline void
watch_list_remove(struct watch_list *wl, uint32_t cref, uint32_t is_bin)
{
	struct watcher *watchers = watch_list_array(wl);
	uint32_t i;
	if (is_bin) {
		for (i = 0; watchers[i].cref != cref; i++);
		wl->n_bin--;
	} else
		for (i = wl->n_bin; watchers[i].cref != cref; i++);
	libmin_assert(i < watch_list_size(wl));
	libmin_memmove((wl->watchers + i), (wl->watchers + i + 1),
	        (wl->size - i - 1) * sizeof(struct watcher));
	wl->size -= 1;
}

static inline vec_wl_t *
vec_wl_alloc(uint32_t cap)
{
	vec_wl_t *vec_wl = STM_ALLOC(vec_wl_t, 1);

	if (cap == 0)
		vec_wl->cap = 4;
	else
		vec_wl->cap = cap;
	vec_wl->size = 0;
	vec_wl->watch_lists = STM_CALLOC(
	    struct watch_list, sizeof(struct watch_list) * vec_wl->cap);
	return vec_wl;
}

static inline void
vec_wl_free(vec_wl_t *vec_wl)
{
	uint32_t i;
	for (i = 0; i < vec_wl->size; i++)
		watch_list_free(vec_wl->watch_lists + i);
	STM_FREE(vec_wl->watch_lists);
	STM_FREE(vec_wl);
}

static inline void
vec_wl_push(vec_wl_t *vec_wl)
{
	if (vec_wl->size == vec_wl->cap) {
		uint32_t new_size =
		    (vec_wl->cap < 4) ? vec_wl->cap * 2 : (vec_wl->cap / 2) * 3;

		vec_wl->watch_lists = STM_REALLOC(
		    struct watch_list, vec_wl->watch_lists, new_size);
		libmin_memset(vec_wl->watch_lists + vec_wl->cap, 0,
		       sizeof(struct watch_list) * (new_size - vec_wl->cap));
		if (vec_wl->watch_lists == NULL) {
			libmin_printf("failed to realloc memory from %.1f mb to %.1f "
			       "mb.\n",
			       1.0 * vec_wl->cap / (1 << 20),
			       1.0 * new_size / (1 << 20));
		}
		vec_wl->cap = new_size;
	}
	vec_wl->size++;
}

static inline struct watch_list *
vec_wl_at(vec_wl_t *vec_wl, uint32_t idx)
{
	libmin_assert(idx < vec_wl->cap);
	libmin_assert(idx < vec_wl->size);
	return vec_wl->watch_lists + idx;
}

#endif /* SATOMI__WATCH_LIST_H */

