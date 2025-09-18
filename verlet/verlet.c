/*
 * verlet.c
 *
 * Minimal Verlet integration benchmark (2D harmonic oscillators).
 * - Deterministic init (fixed LCG seed)
 * - Tunable problem size via macros
 * - No malloc; static storage
 * - Optional timing via RISC-V rdcycle or portable fallbacks
 * - Produces a simple checksum to verify determinism
 *
 * You can compile this as freestanding or hosted. If printf is unavailable,
 * define NO_STDIO and the code will avoid it (writes checksum to a volatile).
 */

#include "libmin.h"

/* ---------------- Tunables (override via -D…) ---------------- */
#ifndef VB_N
#define VB_N  128        /* number of particles */
#endif

#ifndef VB_STEPS
#define VB_STEPS  50    /* number of time steps */
#endif

#ifndef VB_DT
#define VB_DT  1.0e-3     /* time step */
#endif

#ifndef VB_K
#define VB_K   5.0        /* spring constant for a = -k*x */
#endif

#ifndef VB_DIM
#define VB_DIM 2          /* 1, 2, or 3 (we store 2D when VB_DIM==2) */
#endif

#ifndef VB_FP
#define VB_FP double      /* float or double */
#endif

/* ---------------- Deterministic pseudo-random init ---------------- */
static inline uint32_t vb_lcg(uint32_t *state) {
  /* 32-bit LCG (Numerical Recipes constants) */
  *state = (*state) * 1664525u + 1013904223u;
  return *state;
}

static inline VB_FP vb_urand(uint32_t *state) {
  /* Convert to [0,1) as VB_FP */
  return (VB_FP)(vb_lcg(state) & 0x00FFFFFFu) / (VB_FP)16777216.0;
}

/* ---------------- Storage ---------------- */
static VB_FP x[VB_N * VB_DIM];   /* positions */
static VB_FP v[VB_N * VB_DIM];   /* velocities */
static VB_FP a[VB_N * VB_DIM];   /* accelerations */

/* ---------------- Physics kernel ----------------
   Force model: independent harmonic wells, per coordinate:
     a = -k * x
   Integration (position-Verlet + velocity update):
     x(t+dt) = x(t) + v(t)*dt + 0.5*a(t)*dt^2
     a(t+dt) = -k * x(t+dt)
     v(t+dt) = v(t) + 0.5*(a(t) + a(t+dt))*dt
*/
static void vb_init(void) {
  uint32_t rng = 0x12345678u;
  for (int i = 0; i < VB_N; ++i) {
    for (int d = 0; d < VB_DIM; ++d) {
      VB_FP px = (vb_urand(&rng) * (VB_FP)2.0 - (VB_FP)1.0);  /* [-1, 1) */
      VB_FP pv = (vb_urand(&rng) * (VB_FP)2.0 - (VB_FP)1.0);
      x[i*VB_DIM + d] = px;
      v[i*VB_DIM + d] = pv * (VB_FP)0.1;                      /* smaller initial v */
      a[i*VB_DIM + d] = (VB_FP)(-VB_K) * px;
    }
#if VB_DIM < 3
    /* nothing: arrays sized by VB_DIM */
#endif
  }
}

/* Store a_old in a small temp to keep code simple and avoid big memory churn */
static void vb_step_avg(VB_FP dt) {
  const VB_FP half_dt2 = (VB_FP)0.5 * dt * dt;

  /* 1) save a_old in a temp (stack or static) */
  static VB_FP a_old[VB_N * VB_DIM];
  for (int i = 0; i < VB_N * VB_DIM; ++i) a_old[i] = a[i];

  /* 2) x update */
  for (int i = 0; i < VB_N; ++i) {
    for (int d = 0; d < VB_DIM; ++d) {
      int idx = i*VB_DIM + d;
      x[idx] = x[idx] + v[idx]*dt + a_old[idx]*half_dt2;
    }
  }

  /* 3) a_new */
  for (int i = 0; i < VB_N; ++i) {
    for (int d = 0; d < VB_DIM; ++d) {
      int idx = i*VB_DIM + d;
      a[idx] = (VB_FP)(-VB_K) * x[idx];
    }
  }

  /* 4) v update with average accel */
  for (int i = 0; i < VB_N; ++i) {
    for (int d = 0; d < VB_DIM; ++d) {
      int idx = i*VB_DIM + d;
      v[idx] = v[idx] + ((a_old[idx] + a[idx]) * (VB_FP)0.5) * dt;
    }
  }
}

/* ---------------- Checksum for correctness ---------------- */
static uint64_t vb_checksum(void) {
  /* Mix positions and velocities into a 64-bit hash in a stable way. */
  const VB_FP scale = (VB_FP)1e6; /* scale to stabilize floating rounding in cast */
  uint64_t h = 0xcbf29ce484222325ULL; /* FNV-ish seed */
  for (int i = 0; i < VB_N * VB_DIM; ++i) {
    int64_t xi = (int64_t)(x[i] * scale);
    int64_t vi = (int64_t)(v[i] * scale);
    uint64_t ux = (uint64_t)xi;
    uint64_t uv = (uint64_t)vi;
    h ^= ux + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
    h ^= uv + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
  }
  return h;
}

/* ---------------- Entry point ---------------- */
volatile uint64_t vb_sink; /* for NO_STDIO mode */

int main(void) {
  vb_init();

  for (int s = 0; s < VB_STEPS; ++s) {
    vb_step_avg((VB_FP)VB_DT);
  }
  uint64_t sum = vb_checksum();

  libmin_printf("verlet2d: N=%d steps=%d dt=%g k=%g dim=%d fp=%s\n",
         VB_N, VB_STEPS, (double)VB_DT, (double)VB_K, VB_DIM,
         (sizeof(VB_FP)==sizeof(double)) ? "double" : "float");
  libmin_printf("checksum=0x%08x%08x\n", (uint32_t)((sum >> 32)), (uint32_t)(sum & (uint32_t)0xffffffffU));

  libmin_success();
}

