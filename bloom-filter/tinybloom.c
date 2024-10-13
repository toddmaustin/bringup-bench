/* Copyright (C) 2010 by Joseph Kogut

This software is licensed under the MIT license.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.	*/

#include "libmin.h"
#include "tinybloom.h"

#define GETBIT(a, n) ((a[n >> 5] & (1 << (n & 31))) > 0)
#define SETBIT(a, n) (a[n >> 5] |= 1 << (n & 31))

bloom_filter* create_bfilter(size_t size)
{
	bloom_filter* bFilter;
	bFilter = (bloom_filter*)libmin_malloc(sizeof(bloom_filter));

	bFilter->filter_size = (size / (sizeof(unsigned) * 8)) + ((size % sizeof(unsigned) * 8) != 0 ? 1 : 0);

	bFilter->num_buckets = (bFilter->filter_size) * sizeof(unsigned) * 8;

	bFilter->filter = (unsigned*)libmin_malloc((bFilter->filter_size) * sizeof(unsigned));
	libmin_memset(bFilter->filter, 0, bFilter->filter_size);

	return bFilter;
}

void destroy_bfilter(bloom_filter* bFilter)
{
	if(bFilter->filter) libmin_free(bFilter->filter);
	if(bFilter) libmin_free(bFilter);

	bFilter->filter = NULL;
	bFilter = NULL;
}

void bfilter_add(const bloom_filter* bFilter, const unsigned* input)
{
	SETBIT(bFilter->filter, *input % bFilter->num_buckets);
}


int bfilter_check(const bloom_filter* bFilter, const unsigned* input)
{
	return GETBIT(bFilter->filter, *input % bFilter->num_buckets);
}

int bfilter_intersect(bloom_filter* a, bloom_filter* b)
{
	if(a->filter_size != b->filter_size) return 1;

	int i;
	for(i = 0; i < a->filter_size; i++)
		a->filter[i] |= b->filter[i];

	return 0;
}

