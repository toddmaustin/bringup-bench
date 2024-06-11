#include "libmin.h"
#include "libtarg.h"

/* malloc/free functions */

typedef char MEMALIGN[16];

union memhdr {
	struct {
		size_t size;
		unsigned is_free;

		union memhdr *next;
	};
	MEMALIGN stub;
};

typedef union memhdr memhdr_t;

static memhdr_t *head = NULL, *tail = NULL;

static memhdr_t *
__get_free_block(size_t size) {
	memhdr_t *current = head;
	while (current) {
		if (current->is_free && current->size >= size) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void *
libmin_malloc(size_t size) {
	void *block;
	memhdr_t *header;

	if (!size) {
		return NULL;
	}
	header = __get_free_block(size);

	if (header) {
		header->is_free = 0;
		return (void *) (header + 1);
	}
	size_t total_size = sizeof(memhdr_t) + size;
	block = libtarg_sbrk(total_size);
	if (block == (void *) -1) {
		return NULL;
	}

	header = block;
	header->size = size;
	header->is_free = 0;
	header->next = NULL;
	if (!head) {
		head = header;
	}
	if (tail) {
		tail->next = header;
	}
	tail = header;

	return (void *) (header + 1);
}

void
libmin_free(void *block) {
	memhdr_t *header;

	if (!block) {
		return;
	}

	header = (memhdr_t * )block - 1;

	void *pbreak = libtarg_sbrk(0);
	memhdr_t *tmp;
	if ((char *) block + header->size == pbreak) {
		if (head == tail) {
			head = tail = NULL;
		} else {
			tmp = head;
			while (tmp) {
				if (tmp->next == tail) {
					tmp->next = NULL;
					tail = tmp;
				}
				tmp = tmp->next;
			}
		}
		libtarg_sbrk(0 - sizeof(memhdr_t) - header->size);
		return;
	}

	header->is_free = 1;
}

void *
libmin_calloc(size_t num, size_t nsize) {
	if (!num || !nsize) {
		return NULL;
	}

	size_t size = num * nsize;
	if (nsize != size / num) {
		return NULL; // If ml
	}
	void *block = libmin_malloc(size);
	if (!block) {
		return NULL;
	}
	libmin_memset(block, 0, size);
	return block;
}

void *
libmin_realloc(void *block, size_t size)
{
	if (!block || !size) {
		return libmin_malloc(size);
	}

	memhdr_t *header = (memhdr_t *) block - 1;
	if (header->size >= size) {
		return block;
	}

	void *ret = libmin_malloc(size);
	if (ret) {
		libmin_memcpy(ret, block, header->size);
		libmin_free(block);
	}
	return ret;
}
