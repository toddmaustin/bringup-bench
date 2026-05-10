#include "libmin.h"

typedef int8_t VIP_ENCCHAR;
typedef uint64_t VIP_ENCBOOL;
typedef uint64_t VIP_ENCUINT64;
typedef float VIP_ENCFLOAT;
typedef double VIP_ENCDOUBLE;
typedef uint64_t VIP_ENCINT;

#define sizet 800
#define sizec1 400
#define sizec2 400

static VIP_ENCDOUBLE datax[sizet];
static VIP_ENCDOUBLE datay[sizet];
static VIP_ENCDOUBLE dataxC1[sizec1];
static VIP_ENCDOUBLE datayC1[sizec1];
static VIP_ENCDOUBLE dataxC2[sizec2];
static VIP_ENCDOUBLE datayC2[sizec2];
static VIP_ENCDOUBLE dataxC1test[sizec1];
static VIP_ENCDOUBLE datayC1test[sizec1];
static VIP_ENCDOUBLE dataxC2test[sizec2];
static VIP_ENCDOUBLE datayC2test[sizec2];

static VIP_ENCINT classPrediction[sizet];
static VIP_ENCDOUBLE scorec1[sizec1];
static VIP_ENCDOUBLE scorec2[sizec2];

static void
initializeData(double cond1, double cond2)
{
  libmin_srand(10);

  for (int i = 0; i < sizec1; i++)
  {
    datax[i] = (double)(libmin_rand() % 40);
    datay[i] = (VIP_ENCDOUBLE)cond1 * datax[i] + cond2 + (double)(libmin_rand() % 40);

    dataxC1[i] = datax[i];
    datayC1[i] = datay[i];

    dataxC1test[i] = (double)(libmin_rand() % 40);
    datayC1test[i] = (VIP_ENCDOUBLE)cond1 * dataxC1test[i] + cond2 + (double)(libmin_rand() % 40);
  }

  for (int i = 0; i < sizec2; i++)
  {
    int idx = i + sizec1 - 1;
    datax[idx] = (double)(libmin_rand() % 40);
    datay[idx] = (VIP_ENCDOUBLE)cond1 * datax[idx] + cond2 - (double)(libmin_rand() % 40);

    dataxC2[i] = datax[idx];
    datayC2[i] = datay[idx];

    dataxC2test[i] = (double)(libmin_rand() % 40);
    datayC2test[i] = (VIP_ENCDOUBLE)cond1 * dataxC2test[i] + cond2 - (double)(libmin_rand() % 40);
  }

  // Preserve behavior of original VIP-Bench implementation.
  datax[sizet - 1] = 0.0;
  datay[sizet - 1] = 0.0;
}

static VIP_ENCDOUBLE
mean(VIP_ENCDOUBLE *data, int size)
{
  VIP_ENCDOUBLE sum = 0.0;

  for (int i = 0; i < size; i++)
    sum = sum + data[i];

  return sum / (double)size;
}

static VIP_ENCDOUBLE
variance(VIP_ENCDOUBLE *data, VIP_ENCDOUBLE data_mean, int size)
{
  VIP_ENCDOUBLE sum = 0.0;

  for (int i = 0; i < size; i++)
  {
    VIP_ENCDOUBLE dev = data[i] - data_mean;
    sum = sum + dev * dev;
  }

  return sum / (double)size;
}

static VIP_ENCDOUBLE
covariance(VIP_ENCDOUBLE *vx, VIP_ENCDOUBLE *vy, VIP_ENCDOUBLE meanx,
           VIP_ENCDOUBLE meany, int size)
{
  VIP_ENCDOUBLE sum = 0.0;

  for (int i = 0; i < size; i++)
  {
    VIP_ENCDOUBLE devx = vx[i] - meanx;
    VIP_ENCDOUBLE devy = vy[i] - meany;
    sum = sum + devx * devy;
  }

  return sum / (double)size;
}

static void
varcovmatrix(VIP_ENCDOUBLE *vx, VIP_ENCDOUBLE *vy, int size,
             VIP_ENCDOUBLE out[2][2])
{
  VIP_ENCDOUBLE meanx = mean(vx, size);
  VIP_ENCDOUBLE meany = mean(vy, size);
  VIP_ENCDOUBLE varx = variance(vx, meanx, size);
  VIP_ENCDOUBLE vary = variance(vy, meany, size);
  VIP_ENCDOUBLE cov = covariance(vx, vy, meanx, meany, size);

  out[0][0] = varx;
  out[0][1] = cov;
  out[1][0] = cov;
  out[1][1] = vary;
}

static void
wgcov(VIP_ENCDOUBLE dataxc1[], VIP_ENCDOUBLE datayc1[], VIP_ENCDOUBLE dataxc2[],
      VIP_ENCDOUBLE datayc2[], int c1, int c2, VIP_ENCDOUBLE out[2][2])
{
  VIP_ENCDOUBLE m1[2][2];
  VIP_ENCDOUBLE m2[2][2];

  varcovmatrix(dataxc1, datayc1, c1, m1);
  varcovmatrix(dataxc2, datayc2, c2, m2);

  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++)
      out[i][j] = (m1[i][j] * (double)(c1 - 1) + m2[i][j] * (double)(c2 - 1)) /
                  (double)(c1 + c2 - 2);
}

static void
bgcov(VIP_ENCDOUBLE vx[], VIP_ENCDOUBLE vy[], VIP_ENCDOUBLE wg[2][2], int size,
      VIP_ENCDOUBLE out[2][2])
{
  VIP_ENCDOUBLE tot[2][2];
  varcovmatrix(vx, vy, size, tot);

  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++)
      out[i][j] = tot[i][j] - wg[i][j];
}

static VIP_ENCDOUBLE
determinant(VIP_ENCDOUBLE m[2][2])
{
  return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

static void
inverse(VIP_ENCDOUBLE in[2][2], VIP_ENCDOUBLE out[2][2])
{
  VIP_ENCDOUBLE det = determinant(in);

  out[0][0] = in[1][1] / det;
  out[0][1] = -in[0][1] / det;
  out[1][0] = -in[1][0] / det;
  out[1][1] = in[0][0] / det;
}

static void
multiply2x2(VIP_ENCDOUBLE a[2][2], VIP_ENCDOUBLE b[2][2], VIP_ENCDOUBLE out[2][2])
{
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      out[i][j] = 0.0;
      for (int k = 0; k < 2; k++)
        out[i][j] = out[i][j] + a[i][k] * b[k][j];
    }
  }
}

static void
eigenvalue2x2(VIP_ENCDOUBLE m[2][2], VIP_ENCDOUBLE out[2])
{
  VIP_ENCDOUBLE b = -(m[0][0] + m[1][1]);
  VIP_ENCDOUBLE c = m[0][0] * m[1][1] - m[0][1] * m[1][0];
  VIP_ENCDOUBLE disc = b * b - (VIP_ENCDOUBLE)4.0 * c;
  VIP_ENCDOUBLE root = libmin_sqrt(disc);

  out[0] = (-b + root) / (VIP_ENCDOUBLE)2.0;
  out[1] = (-b - root) / (VIP_ENCDOUBLE)2.0;
}

static void
eigenvector2x2(VIP_ENCDOUBLE m[2][2], VIP_ENCDOUBLE eig, VIP_ENCDOUBLE out[2])
{
  VIP_ENCDOUBLE denom = libmin_sqrt((m[0][0] - eig) * (m[0][0] - eig) + m[0][1] * m[0][1]);
  out[0] = m[0][1] / denom;
  out[1] = -(m[0][0] - eig) / denom;
}

static void
calcScore(VIP_ENCDOUBLE b0, VIP_ENCDOUBLE b1)
{
  for (int i = 0; i < sizec1; i++)
    scorec1[i] = b0 * dataxC1[i] + b1 * datayC1[i];

  for (int i = 0; i < sizec2; i++)
    scorec2[i] = b0 * dataxC2[i] + b1 * datayC2[i];
}

static void
normalizeCoeff(VIP_ENCDOUBLE *b0, VIP_ENCDOUBLE *b1)
{
  calcScore(*b0, *b1);

  VIP_ENCDOUBLE vars1 = variance(scorec1, mean(scorec1, sizec1), sizec1);
  VIP_ENCDOUBLE vars2 = variance(scorec2, mean(scorec2, sizec2), sizec2);
  VIP_ENCDOUBLE pooled = (vars1 * (double)(sizec1 - 1) + vars2 * (double)(sizec2 - 1)) /
                         (double)(sizec1 + sizec2 - 2);

  VIP_ENCDOUBLE norm = libmin_sqrt(pooled);
  *b0 = *b0 / norm;
  *b1 = *b1 / norm;
}

int
main(void)
{
  libmin_srand(42);

  VIP_ENCDOUBLE b0 = 0.0;
  VIP_ENCDOUBLE b1 = 0.0;

  initializeData(3.0, 4.0);

  VIP_ENCDOUBLE meanx = mean(datax, sizet);
  VIP_ENCDOUBLE meany = mean(datay, sizet);
  VIP_ENCINT path = 1;

  {
    VIP_ENCDOUBLE wg[2][2];
    VIP_ENCDOUBLE bg[2][2];
    VIP_ENCDOUBLE s[2][2];
    VIP_ENCDOUBLE inv_wg[2][2];
    VIP_ENCDOUBLE eigval[2];
    VIP_ENCDOUBLE eigvec[2];

    wgcov(dataxC1, datayC1, dataxC2, datayC2, sizec1, sizec2, wg);
    bgcov(datax, datay, wg, sizet, bg);
    inverse(wg, inv_wg);
    multiply2x2(inv_wg, bg, s);
    eigenvalue2x2(s, eigval);
    eigenvector2x2(s, eigval[0], eigvec);

    b0 = eigvec[0];
    b1 = eigvec[1];
    normalizeCoeff(&b0, &b1);

    VIP_ENCINT c1pos = 0;
    VIP_ENCINT c2pos = 0;

    for (int i = 0; i < sizec1; i++)
    {
      scorec1[i] = b0 * (dataxC1[i] - meanx) + b1 * (datayC1[i] - meany);
      VIP_ENCBOOL cond = scorec1[i] >= 0.0;
      c1pos = cond ? c1pos + (VIP_ENCINT)1 : c1pos;
    }

    for (int i = 0; i < sizec2; i++)
    {
      scorec2[i] = b0 * (dataxC2[i] - meanx) + b1 * (datayC2[i] - meany);
      VIP_ENCBOOL cond = scorec2[i] >= 0.0;
      c2pos = cond ? c2pos + (VIP_ENCINT)1 : c2pos;
    }

    path = (c1pos < c2pos) ? (VIP_ENCINT)2 : (VIP_ENCINT)1;
  }

  for (int i = 0; i < sizec1; i++)
  {
    VIP_ENCDOUBLE score = b0 * (dataxC1test[i] - meanx) + b1 * (datayC1test[i] - meany);
    VIP_ENCBOOL cond = (score >= 0.0 && path == (VIP_ENCINT)1) ||
                       (score < 0.0 && path == (VIP_ENCINT)2);
    classPrediction[i] = cond ? (VIP_ENCINT)1 : (VIP_ENCINT)2;
  }

  for (int i = 0; i < sizec2; i++)
  {
    int idx = i + sizec1 - 1;
    VIP_ENCDOUBLE score = b0 * (dataxC2test[i] - meanx) + b1 * (datayC2test[i] - meany);
    VIP_ENCBOOL cond = (score >= 0.0 && path == (VIP_ENCINT)1) ||
                       (score < 0.0 && path == (VIP_ENCINT)2);
    classPrediction[idx] = cond ? (VIP_ENCINT)1 : (VIP_ENCINT)2;
  }

  int c1ok = 0;
  int c2ok = 0;
  for (int i = 0; i < sizec1; i++)
    if (classPrediction[i] == 1)
      c1ok++;
  for (int i = 0; i < sizec2; i++)
    if (classPrediction[i + sizec1 - 1] == 2)
      c2ok++;

  libmin_printf("LDA results: path=%llu c1=%d/%d c2=%d/%d b0=%lf b1=%lf\n",
                (unsigned long long)path, c1ok, sizec1, c2ok, sizec2,
                b0, b1);

  libmin_success();
  return 0;
}
