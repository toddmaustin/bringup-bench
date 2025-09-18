// bench_horner_fp64.c
// Simple FP64 Horner benchmark: y = (((cD*x + cD-1)*x + ... )*x + c0)
// No CLI, no timing. Adjust work via macros.

// Knobs (override with -DNAME=value):
#ifndef N_ITER
#define N_ITER      100
#endif
#ifndef N_ELEMS
#define N_ELEMS     (1u << 6)   // 1,048,576
#endif
#ifndef DEGREE
#define DEGREE      16           // polynomial degree (>=1)
#endif
#ifndef STRIDE
#define STRIDE      1
#endif
#ifndef SEED
#define SEED        1u
#endif

#include "libmin.h"

#define fma(x,y,z) (((x)*(y)) + (z))

static uint64_t rng; // xorshift64
static inline uint64_t xr(void){ uint64_t x=rng; x^=x<<13; x^=x>>7; x^=x<<17; return rng=x; }
static inline double urand01(void){ return (double)((xr()>>11) * (1.0/9007199254740992.0)); } // [0,1)

int
main(void)
{
  const size_t n = (size_t)N_ELEMS, stride = (size_t)STRIDE;
  rng = (SEED?SEED:1);

  // Inputs and outputs
  double *x = (double*)libmin_malloc(n*sizeof(double));
  double *y = (double*)libmin_malloc(n*sizeof(double));
  if(!x || !y){ libmin_printf("alloc failed\n"); libmin_fail(1); }

  // Coefficients c[0..DEGREE] in a reasonable range
  double *c = (double*)libmin_malloc((DEGREE+1)*sizeof(double));
  if(!c){ libmin_printf("alloc failed\n"); libmin_fail(1); }
  for(int k=0;k<=DEGREE;k++){ c[k] = -1.0 + 2.0*urand01(); }

  // Inputs in [-1,1] to keep values bounded
  for(size_t i=0, idx=0;i<n;i++, idx=(idx+stride)%n){
    x[idx] = -1.0 + 2.0*urand01();
    y[idx] = 0.0;
  }

  volatile double sink = 0.0;

  for(uint64_t it=0; it<(uint64_t)N_ITER; ++it){
    for(size_t i=0, idx=0;i<n;i++, idx=(idx+stride)%n){
      double acc = c[DEGREE];
      // Horner loop: DEGREE FMAs (or mul+add if fma not used)
      for(int k=DEGREE-1;k>=0;k--)
        acc = fma(acc, x[idx], c[k]);
      y[idx] = acc;
      sink += acc * 1e-16;
    }
  }

  // Report
  const double ops = (double)N_ITER * (double)n * (double)(2*DEGREE); // FMA counted as 2 flops
  double checksum = 0.0; for(size_t i=0;i<n;i++) checksum += y[i];

  libmin_printf("horner64 degree=%d N_ITER=%u n=%u stride=%u ops=%f checksum=%f\n",
         DEGREE, (unsigned)N_ITER, n, stride, ops, checksum);

  libmin_free(c); libmin_free((void*)x); libmin_free((void*)y);
  return 0;
}

