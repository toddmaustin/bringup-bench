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

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>

#if defined(__APPLE__) || defined(__MACH__)
#include <mach/clock.h>
#include <mach/mach.h>
#endif

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

static inline double 
stm_clock()
{
	struct timespec ts;
	double ret;

#if defined(__APPLE__) || defined(__MACH__)
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts.tv_sec = mts.tv_sec;
	ts.tv_nsec = mts.tv_nsec;
#else
	if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts) < 0) 
		return -1;
#endif

	ret =  (double) ts.tv_sec;
	ret += ((double) ts.tv_nsec) / 1000000000;
	return ret;
}

static int 
mkdir_p(const char *path)
{
	char _path[PATH_MAX];
	char *p; 

	strcpy(_path, path);
	for (p = _path + 1; *p; p++) {
		if (*p == '/') {
			*p = '\0';
			if (mkdir(_path, S_IRWXU) != 0) {
				if (errno != EEXIST)
					return -1; 
			}
			*p = '/';
		}
	}   
	if (mkdir(_path, S_IRWXU) != 0) {
		if (errno != EEXIST)
			return -1; 
	}   
	return 0;
}

#endif /* SATOMI__UTILS__MISC_H */

