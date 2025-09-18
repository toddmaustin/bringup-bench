/*
 * bringup_bench_cg.c
 *
 * Minimal Conjugate Gradient solver benchmark for Bringup-Bench
 * Solves Ax = b where A is a 2D Laplacian (5-point stencil).
 *
 * - Deterministic init
 * - Tunable grid size (N*N system)
 * - Sparse CSR matrix
 * - Iterative solver with fixed max iterations
 * - Reports checksum of solution
 */

#include "libmin.h"

/* ---------------- Tunables ---------------- */
#ifndef CG_N
#define CG_N 32              /* grid dimension (matrix size = N*N) */
#endif

#ifndef CG_MAXN
#define CG_MAXN (CG_N*CG_N)  /* maximum problem size */
#endif

#ifndef CG_ITERS
#define CG_ITERS 25          /* number of CG iterations */
#endif

#ifndef CG_FP
#define CG_FP double         /* float or double */
#endif

/* ---------------- Storage ---------------- */
static int    row_ptr[CG_MAXN+1];
static int    col_idx[CG_MAXN*5];   /* 5 nonzeros per row */
static CG_FP  val[CG_MAXN*5];       /* values */

static CG_FP  x[CG_MAXN];
static CG_FP  b[CG_MAXN];
static CG_FP  r[CG_MAXN];
static CG_FP  p[CG_MAXN];
static CG_FP  Ap[CG_MAXN];

/* ---------------- Simple RNG for init ---------------- */
static uint32_t cg_rng = 0x12345678u;
static inline uint32_t cg_lcg(void) {
    cg_rng = cg_rng * 1664525u + 1013904223u;
    return cg_rng;
}
static inline CG_FP cg_urand(void) {
    return (CG_FP)(cg_lcg() & 0xFFFFFF) / (CG_FP)16777216.0;
}

/* ---------------- Sparse Laplacian Matrix ---------------- */
static void cg_build_matrix(int n) {
    int N = n*n;
    int nnz = 0;
    for (int i = 0; i < N; i++) {
        row_ptr[i] = nnz;
        int row = i / n;
        int col = i % n;

        // center
        col_idx[nnz] = i; val[nnz++] = 4.0;

        // neighbors
        if (row > 0)    { col_idx[nnz] = i-n; val[nnz++] = -1.0; }
        if (row < n-1)  { col_idx[nnz] = i+n; val[nnz++] = -1.0; }
        if (col > 0)    { col_idx[nnz] = i-1; val[nnz++] = -1.0; }
        if (col < n-1)  { col_idx[nnz] = i+1; val[nnz++] = -1.0; }
    }
    row_ptr[N] = nnz;
}

/* ---------------- Sparse MatVec: y = A*x ---------------- */
static void cg_spmv(CG_FP *y, const CG_FP *x, int N) {
    for (int i = 0; i < N; i++) {
        CG_FP sum = 0;
        for (int j = row_ptr[i]; j < row_ptr[i+1]; j++) {
            sum += val[j] * x[col_idx[j]];
        }
        y[i] = sum;
    }
}

/* ---------------- Vector ops ---------------- */
static inline CG_FP cg_dot(const CG_FP *a, const CG_FP *b, int N) {
    CG_FP s = 0;
    for (int i = 0; i < N; i++) s += a[i]*b[i];
    return s;
}
static inline void cg_axpy(CG_FP *y, const CG_FP *x, CG_FP alpha, int N) {
    for (int i = 0; i < N; i++) y[i] += alpha * x[i];
}
static inline void cg_copy(CG_FP *dst, const CG_FP *src, int N) {
    for (int i = 0; i < N; i++) dst[i] = src[i];
}

/* ---------------- Conjugate Gradient ---------------- */
static void cg_solve(int n) {
    int N = n*n;

    // init vectors
    for (int i = 0; i < N; i++) {
        x[i] = 0.0;
        b[i] = cg_urand();
    }

    cg_spmv(Ap, x, N);
    for (int i = 0; i < N; i++) {
        r[i] = b[i] - Ap[i];
        p[i] = r[i];
    }

    CG_FP rsold = cg_dot(r,r,N);

    for (int k = 0; k < CG_ITERS; k++) {
        cg_spmv(Ap, p, N);
        CG_FP alpha = rsold / cg_dot(p,Ap,N);
        cg_axpy(x, p, alpha, N);
        cg_axpy(r, Ap, -alpha, N);

        CG_FP rsnew = cg_dot(r,r,N);
        if (rsnew < 1e-12) break;

        CG_FP beta = rsnew / rsold;
        for (int i = 0; i < N; i++)
            p[i] = r[i] + beta*p[i];

        rsold = rsnew;
    }
}

/* ---------------- Checksum ---------------- */
static uint64_t cg_checksum(int N) {
    uint64_t h = 0x9e3779b97f4a7c15ULL;
    for (int i = 0; i < N; i++) {
        int64_t xi = (int64_t)(x[i]*1e6);
        h ^= (uint64_t)xi + (h<<6) + (h>>2);
    }
    return h;
}

/* ---------------- Entry ---------------- */
volatile uint64_t cg_sink;

int main(void) {
    cg_build_matrix(CG_N);
    cg_solve(CG_N);
    uint64_t sum = cg_checksum(CG_N*CG_N);

    libmin_printf("cg: N=%d iters=%d checksum=0x%08x%08x\n",
           CG_N, CG_ITERS, (uint32_t)(sum >> 32), (uint32_t)sum);

    libmin_success();
}

