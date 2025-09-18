// bench_transcendentals.c
// Bringup-Bench style transcendentals benchmark (no CLI, no timing).
// 64-bit only; functions: {exp, sin, cos, mix(exp/sin/cos)}.
// Work per experiment: N_ITER * N_ELEMS.

// Compile-time knobs (override with -DNAME=value):
#ifndef N_ITER
#define N_ITER      10            // outer iterations of the kernel
#endif
#ifndef N_ELEMS
#define N_ELEMS     (1u << 6)     // elements per array (1,048,576)
#endif
#ifndef STRIDE
#define STRIDE      1              // memory stride (in elements)
#endif
#ifndef SEED
#define SEED        1u             // RNG seed for input init
#endif

#include "libmin.h"

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

typedef enum { FN_EXP, FN_SIN, FN_COS, FN_MIX, FN_COUNT } fn_t;
static const char* FN_NAMES[] = {"exp","sin","cos","mix"};

// --------------------------------- RNG ---------------------------------------
static uint64_t xorshift64(uint64_t *s) {
  uint64_t x = *s;
  x ^= x << 13; x ^= x >> 7; x ^= x << 17; *s = x;
  return x;
}

// --------------------- Input initialization (domain-safe) --------------------
static void init_inputs_f64(double* x, size_t n, size_t stride, fn_t fn, uint64_t seed) {
  uint64_t s = seed ? seed : 1;
  for (size_t i=0, idx=0; i<n; i++, idx=(idx+stride)%n) {
    double u = (double)((xorshift64(&s) >> 11) * (1.0/9007199254740992.0)); // [0,1)
    switch (fn) {
      case FN_EXP:  x[idx] = -10.0 + 20.0 * u; break;            // [-10,10]
      case FN_SIN:
      case FN_COS:  x[idx] = -M_PI + 2.0*M_PI * u; break;        // [-pi,pi]
      case FN_MIX:  x[idx] = -4.0 + 8.0 * u; break;              // general range
      default:      x[idx] = u;
    }
  }
}

// ------------------------------ Kernel (FP64) --------------------------------
static double run_fp64(fn_t fn) {
  const size_t n = (size_t)N_ELEMS, stride = (size_t)STRIDE;
  double *x = (double*)libmin_malloc(n*sizeof(double));
  double *y = (double*)libmin_malloc(n*sizeof(double));
  if (!x || !y) { libmin_printf("alloc failed\n"); libmin_fail(1); }
  init_inputs_f64(x, n, stride, fn, (uint64_t)SEED);

  volatile double sink = 0.0;

  for (uint64_t it=0; it < (uint64_t)N_ITER; ++it) {
    for (size_t i=0, idx=0; i<n; i++, idx=(idx+stride)%n) {
      double v = x[idx]; double r;
      switch (fn) {
        case FN_EXP:  r = libmin_exp(v); break;
        case FN_SIN:  r = libmin_sin(v); break;
        case FN_COS:  r = libmin_cos(v); break;
        case FN_MIX: {
          int sel = (int)(i % 3); // 0:exp, 1:sin, 2:cos
          r = (sel==0)?libmin_exp(v):(sel==1)?libmin_sin(v):libmin_cos(v);
        } break;
        default: r = v;
      }
      y[idx] = r; sink += r * 1e-16;
    }
  }

  const double ops = (double)N_ITER * (double)n;   // one transcendental per element
  double checksum = 0.0; for (size_t i=0;i<n;i++) checksum += y[i];

  libmin_printf("transcendentals fn=%s prec=fp64 N_ITER=%u n=%u stride=%u ops=%f checksum=%f\n",
         FN_NAMES[(int)fn], (unsigned)N_ITER, n, stride, ops, checksum);

  libmin_free((void*)x); libmin_free((void*)y);
  return checksum + sink;
}

// --------------------------------- Main -------------------------------------
int main(void) {
  for (int f = 0; f < FN_COUNT; ++f) {
    (void)run_fp64((fn_t)f);
  }
  libmin_success();
}

