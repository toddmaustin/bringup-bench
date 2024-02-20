/* miller_rabin_int.c -- long long implementation of the Miller-Rabin test
 *
 * Copyright 2014 by Colin Benner <colin-software@yzhs.de>
 *
 * This file is part of frobenius-test.
 *
 * frobenius-test is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * frobenius-test is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with frobenius-test.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "libmin.h"

// precision of the primality test, there p_failure = 1/4^K
#define K 16

// primality results
#define PT_COMPOSITE    0
#define PT_PRIME        1
#define PT_PRIME_LIKELY 2

/*
 * Raise b to the e'th power modulo m.  This uses 64-bit registers to hold the
 * results of the multipliations.  Therefore, the results will be wrong if m is
 * greater than 2^32-1
 */
static uint64_t powm(uint64_t b, uint64_t e, uint32_t m)
{
	uint64_t result = 1;

	while (e != 0) {
		if ((e & 1) == 1)
			result = (result * b) % m;
		b = (b * b) % m;
		e /= 2;
	}
	return result;
}

/*
 * This function generates a random integer between in the interval
 * [low, high].  As we divide by (high - low + 1) in the process, we need
 * low < high.
 */
uint64_t get_random_int(uint64_t low, uint64_t high)
{
	return (uint64_t)libmin_rand() % (high - low + 1) + low;
}

/*
 * Calculate s, d such that n-1=2^s*d where d is odd.
 */
void split_int(uint64_t *s, uint64_t *d, uint64_t n)
{
	*s = 0;
	*d = n - 1;

	while ((*d & 1) == 0)
  {
		(*s)++;
		*d /= 2;
	}
}

/*
 * This function checks whether a given number n is a prime or not, using the
 * Miller-Rabin primality test.  This is a probabilistic test which randomly
 * chooses an integer a as a base and checks whether n satisfies a certain
 * property (which depends on b).  If it does, n is a prime for at least three
 * out of four of the possible values of a, if it does not, it is certainly not
 * prime.
 *
 * The implementation is taken from the pseudo code found on
 * http://en.wikipedia.org/wiki/Miller-Rabin_primality_test.
 *
 * The function returns `probably_prime` if it found no evidence, that n might
 * be composite and `composite` if it did find a counter example.
 */
int32_t
miller_rabin_int(uint32_t n, uint32_t k)
{
	uint64_t s;
	uint64_t a = 0, d, x, nm1;
	/* We need an odd integer greater than 3 */
	if ((n & 1) == 0)
		return n == 2 ? PT_PRIME : PT_COMPOSITE;
	if (n == 3)
		return PT_PRIME;
	else if (n < 3)
		return PT_COMPOSITE;

	nm1 = n - 1;

	/* compute s and d s.t. n-1=2^s*d */
	split_int(&s, &d, n);

	/* Repeat the test itself k times to increase the accuracy */
	for (unsigned i = 0; i < k; i++) {
		a = get_random_int(2, n - 2);

		/* compute a^d mod n */
		x = powm(a, d, n);

		if (x == 1 || x == nm1)
			continue;

		for (uint64_t r = 1; r <= s; r++) {
			x = (x * x) % n;
			if (x == 1)
				return PT_COMPOSITE;
			if (x == nm1)
				break;
		}

		if (x != nm1)
			return PT_COMPOSITE;
	}

	return PT_PRIME_LIKELY;
}

// blind queue for results
#define Q_SIZE 64
struct {
  uint32_t val;
  int32_t prim;
} q[Q_SIZE];
int q_head = 0;

int
main(void)
{
  // initialize the RNG
  libmin_srand(42);

  // locate primes in a stream of random numbers
  {
    uint32_t val = 3;
    for (int i=0; i < 200; i++)
    {
      int32_t prim = miller_rabin_int(val, K);
      int _pred = (prim != PT_COMPOSITE);
      if (_pred)
      {
        q[q_head].val = val;
        q[q_head].prim = prim;
        if (q_head+1 < Q_SIZE)
          q_head++;
      }
      val = libmin_rand();
    } 
  }

  // print out the primes that were found
  libmin_printf("Primality tests found %d primes...\n", q_head);
  for (int i=0; i < q_head; i++)
  {
    if (q[i].prim == PT_PRIME)
      libmin_printf("Value %u is `prime' with failure probability (0)\n", q[i].val);
    else if (q[i].prim == PT_PRIME_LIKELY)
      libmin_printf("Value %u is `likely prime' with failure probability (1 in %.0lf)\n", q[i].val, libmin_pow(4.0, K));
  }

  libmin_success();
  return 0;
}
