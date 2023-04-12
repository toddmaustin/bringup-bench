/* This program is evil - please do not show it to children or */
/* undergraduate programming students */
/* Thrashing the Cache to learn memory latencies */

#include "libcosim.h"

#if defined(ALLHITS)
#define OFFSET 1
#elif defined(ALLMISSES)
#define OFFSET 128
#else
#error OFFSET must be specified
#endif

#define ASIZE (64*OFFSET)

char A[ASIZE];

int newmain(void) {

  unsigned int n = 50, i, sum;
  char *p, *q, *r, *s, *t, *u, *v, *w;

  sum = 0;
  p = A + 0*OFFSET;
  q = A + 8*OFFSET;
  r = A + 16*OFFSET;
  s = A + 24*OFFSET;
  t = A + 32*OFFSET;
  u = A + 40*OFFSET;
  v = A + 48*OFFSET;
  w = A + 56*OFFSET;

  for (i=0; i < n; i++)
    {
      sum += p[0*OFFSET];
      sum += p[1*OFFSET];
      sum += p[2*OFFSET];
      sum += p[3*OFFSET];
      sum += p[4*OFFSET];
      sum += p[5*OFFSET];
      sum += p[6*OFFSET];
      sum += p[7*OFFSET];

      sum += q[0*OFFSET];
      sum += q[1*OFFSET];
      sum += q[2*OFFSET];
      sum += q[3*OFFSET];
      sum += q[4*OFFSET];
      sum += q[5*OFFSET];
      sum += q[6*OFFSET];
      sum += q[7*OFFSET];

      sum += r[0*OFFSET];
      sum += r[1*OFFSET];
      sum += r[2*OFFSET];
      sum += r[3*OFFSET];
      sum += r[4*OFFSET];
      sum += r[5*OFFSET];
      sum += r[6*OFFSET];
      sum += r[7*OFFSET];

      sum += s[0*OFFSET];
      sum += s[1*OFFSET];
      sum += s[2*OFFSET];
      sum += s[3*OFFSET];
      sum += s[4*OFFSET];
      sum += s[5*OFFSET];
      sum += s[6*OFFSET];
      sum += s[7*OFFSET];

      sum += t[0*OFFSET];
      sum += t[1*OFFSET];
      sum += t[2*OFFSET];
      sum += t[3*OFFSET];
      sum += t[4*OFFSET];
      sum += t[5*OFFSET];
      sum += t[6*OFFSET];
      sum += t[7*OFFSET];

      sum += u[0*OFFSET];
      sum += u[1*OFFSET];
      sum += u[2*OFFSET];
      sum += u[3*OFFSET];
      sum += u[4*OFFSET];
      sum += u[5*OFFSET];
      sum += u[6*OFFSET];
      sum += u[7*OFFSET];

      sum += v[0*OFFSET];
      sum += v[1*OFFSET];
      sum += v[2*OFFSET];
      sum += v[3*OFFSET];
      sum += v[4*OFFSET];
      sum += v[5*OFFSET];
      sum += v[6*OFFSET];
      sum += v[7*OFFSET];

      sum += w[0*OFFSET];
      sum += w[1*OFFSET];
      sum += w[2*OFFSET];
      sum += w[3*OFFSET];
      sum += w[4*OFFSET];
      sum += w[5*OFFSET];
      sum += w[6*OFFSET];
      sum += w[7*OFFSET];
    }
  return sum;
}

