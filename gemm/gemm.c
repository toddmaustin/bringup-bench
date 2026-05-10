#include "libmin.h"

#define M 32
#define N 32
#define K 32

#define I_ALPHA 3
#define I_BETA  2

#define D_ALPHA 1.5
#define D_BETA  0.5

/* integer GEMM state */
static int64_t IA[M][K];
static int64_t IA_ref[M][K];
static int64_t IB[K][N];
static int64_t IB_ref[K][N];
static int64_t IC[M][N];
static int64_t IC_ref[M][N];

/* double GEMM state */
static double DA[M][K];
static double DA_ref[M][K];
static double DB[K][N];
static double DB_ref[K][N];
static double DC[M][N];
static double DC_ref[M][N];

static void
init_int_matrices(void)
{
  for (int i = 0; i < M; i++)
  {
    for (int k = 0; k < K; k++)
    {
      int64_t init = (int64_t)(((i * 17 + k * 13 + 1) % 31) - 15);
      IA[i][k] = init;
      IA_ref[i][k] = init;
    }
  }

  for (int k = 0; k < K; k++)
  {
    for (int j = 0; j < N; j++)
    {
      int64_t init = (int64_t)(((k * 7 + j * 19 + 3) % 29) - 14);
      IB[k][j] = init;
      IB_ref[k][j] = init;
    }
  }

  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      int64_t init = (int64_t)(((i * 5 + j * 11 + 7) % 23) - 11);
      IC[i][j] = init;
      IC_ref[i][j] = init;
    }
  }
}

static void
int_gemm_kernel(void)
{
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      int64_t acc = 0;

      for (int k = 0; k < K; k++)
        acc += IA[i][k] * IB[k][j];

      IC[i][j] = ((int64_t)I_ALPHA * (int64_t)acc) + ((int64_t)I_BETA * IC[i][j]);
    }
  }
}

static void
int_gemm_reference(void)
{
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
      IC_ref[i][j] *= (int64_t)I_BETA;
  }

  for (int i = 0; i < M; i++)
  {
    for (int k = 0; k < K; k++)
    {
      int64_t aik = (int64_t)I_ALPHA * IA_ref[i][k];

      for (int j = 0; j < N; j++)
        IC_ref[i][j] += aik * IB_ref[k][j];
    }
  }
}

static uint64_t
int_checksum(void)
{
  uint64_t checksum = 0;
  const uint64_t mod = 1000000007ul;

  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      uint64_t term = IC[i][j] + 100000l;
      checksum = ((checksum * 131ul) + term) % mod;
    }
  }

  return checksum;
}

static int
run_int_gemm(void)
{
  init_int_matrices();

  libtarg_start_perf();
  int_gemm_kernel();
  libtarg_stop_perf();

  int_gemm_reference();

  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      if ((IC[i][j]) != IC_ref[i][j])
      {
        libmin_printf("ERROR: int GEMM mismatch at (%d, %d), got %ld expected %ld\n",
                      i, j, (IC[i][j]), IC_ref[i][j]);
        return 1;
      }
    }
  }

  libmin_printf("INFO: int64 GEMM verified, checksum=0x%08lx\n", int_checksum());
  return 0;
}

static void
init_fp_matrices(void)
{
  /*
   * These are chosen to be exact binary fractions, so the reference
   * and main kernel should match exactly even with a different loop order.
   */
  for (int i = 0; i < M; i++)
  {
    for (int k = 0; k < K; k++)
    {
      double init = (double)(((i * 9 + k * 5 + 1) % 17) - 8) / 8.0;
      DA[i][k] = init;
      DA_ref[i][k] = init;
    }
  }

  for (int k = 0; k < K; k++)
  {
    for (int j = 0; j < N; j++)
    {
      double init = (double)(((k * 3 + j * 7 + 2) % 19) - 9) / 16.0;
      DB[k][j] = init;
      DB_ref[k][j] = init;
    }
  }

  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      double init = (double)(((i * 4 + j * 6 + 3) % 13) - 6) / 4.0;
      DC[i][j] = init;
      DC_ref[i][j] = init;
    }
  }
}

static void
fp_gemm_kernel(void)
{
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      double acc = 0.0;

      for (int k = 0; k < K; k++)
        acc += DA[i][k] * DB[k][j];

      DC[i][j] = D_ALPHA * acc + D_BETA * DC[i][j];
    }
  }
}

static void
fp_gemm_reference(void)
{
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
      DC_ref[i][j] *= D_BETA;
  }

  for (int i = 0; i < M; i++)
  {
    for (int k = 0; k < K; k++)
    {
      double aik = D_ALPHA * DA_ref[i][k];

      for (int j = 0; j < N; j++)
        DC_ref[i][j] += aik * DB_ref[k][j];
    }
  }
}

static uint64_t
fp_checksum(void)
{
  uint64_t checksum = 0;
  const uint64_t mod = 1000000007ul;

  /*
   * All results are exact multiples of 1/256, so scale and hash
   * as integers for a stable printed checksum.
   */
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      uint64_t scaled = (uint64_t)(DC[i][j] * 256.0);
      uint64_t term = scaled + 100000lu;
      checksum = ((checksum * 131ul) + term) % mod;
    }
  }

  return checksum;
}

static int
run_fp_gemm(void)
{
  init_fp_matrices();

  libtarg_start_perf();
  fp_gemm_kernel();
  libtarg_stop_perf();

  fp_gemm_reference();

  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      if ((DC[i][j]) != DC_ref[i][j])
      {
        libmin_printf(
          "ERROR: double GEMM mismatch at (%d, %d)\n",
          i, j);
        return 1;
      }
    }
  }

  libmin_printf("INFO: double GEMM verified,  checksum=0x%08lx\n", fp_checksum());
  return 0;
}

int
main(void)
{
  // initialize the pseudo-RNG
  libmin_srand(142);

  if (run_int_gemm())
  {
    libmin_fail(1);
    return 1;
  }

  if (run_fp_gemm())
  {
    libmin_fail(1);
    return 1;
  }

  libmin_printf("INFO: all GEMM tests passed\n");
  libmin_success();
  return 0;
}
