#include "libmin.h"

#define PCA_SAMPLES 192
#define PCA_FEATURES 6
#define PCA_COMPONENTS 2
#define PCA_POWER_ITERS 20

typedef double fp64_t;

static fp64_t data[PCA_SAMPLES][PCA_FEATURES];

static void
init_secret_dataset(void)
{
  for (uint64_t i = 0; i < PCA_SAMPLES; ++i)
  {
    for (uint64_t j = 0; j < PCA_FEATURES; ++j)
    {
      double trend = (double)(((i * 17) + (j * 31) + 13) % 97) * 0.07;
      double band = (double)((i + (j * 3)) % 11) * 0.11;
      double interaction = (double)(((i * (j + 3)) + 5) % 29) * 0.05;
      double bias = ((double)j * 0.37) - 1.25;
      data[i][j] = trend + band + interaction + bias;
    }
  }
}

static fp64_t
dot_vec(const fp64_t a[PCA_FEATURES], const fp64_t b[PCA_FEATURES])
{
  fp64_t sum = 0.0;
  for (uint64_t k = 0; k < PCA_FEATURES; ++k)
    sum = sum + (a[k] * b[k]);
  return sum;
}

static void
mat_vec_mul(const fp64_t mat[PCA_FEATURES][PCA_FEATURES], const fp64_t vec[PCA_FEATURES], fp64_t out[PCA_FEATURES])
{
  for (uint64_t i = 0; i < PCA_FEATURES; ++i)
  {
    fp64_t sum = 0.0;
    for (uint64_t j = 0; j < PCA_FEATURES; ++j)
      sum = sum + (mat[i][j] * vec[j]);
    out[i] = sum;
  }
}

static void
normalize_vec(fp64_t vec[PCA_FEATURES])
{
  fp64_t n2 = dot_vec(vec, vec);
  fp64_t inv_n = 1.0 / libmin_sqrt(n2 + (fp64_t)(1e-12));
  for (uint64_t k = 0; k < PCA_FEATURES; ++k)
    vec[k] = vec[k] * inv_n;
}

static void
compute_means(fp64_t means[PCA_FEATURES])
{
  for (uint64_t j = 0; j < PCA_FEATURES; ++j)
  {
    fp64_t sum = 0.0;
    for (uint64_t i = 0; i < PCA_SAMPLES; ++i)
      sum = sum + data[i][j];
    means[j] = sum / (fp64_t)(PCA_SAMPLES);
  }
}

static void
compute_covariance(const fp64_t means[PCA_FEATURES], fp64_t cov[PCA_FEATURES][PCA_FEATURES])
{
  for (uint64_t a = 0; a < PCA_FEATURES; ++a)
  {
    for (uint64_t b = 0; b < PCA_FEATURES; ++b)
    {
      fp64_t sum = 0.0;
      for (uint64_t i = 0; i < PCA_SAMPLES; ++i)
      {
        fp64_t ca = data[i][a] - means[a];
        fp64_t cb = data[i][b] - means[b];
        sum = sum + (ca * cb);
      }
      cov[a][b] = sum / (fp64_t)(PCA_SAMPLES - 1);
    }
  }
}

static void
power_iteration(const fp64_t cov[PCA_FEATURES][PCA_FEATURES], fp64_t vec[PCA_FEATURES], fp64_t *eig)
{
  for (uint64_t iter = 0; iter < PCA_POWER_ITERS; ++iter)
  {
    fp64_t next[PCA_FEATURES];
    mat_vec_mul(cov, vec, next);

    for (uint64_t k = 0; k < PCA_FEATURES; ++k)
      vec[k] = next[k];

    normalize_vec(vec);
  }

  fp64_t cov_vec[PCA_FEATURES];
  mat_vec_mul(cov, vec, cov_vec);
  *eig = dot_vec(vec, cov_vec);
}

static void
deflate_covariance(fp64_t cov[PCA_FEATURES][PCA_FEATURES], const fp64_t vec[PCA_FEATURES], fp64_t eig)
{
  for (uint64_t a = 0; a < PCA_FEATURES; ++a)
    for (uint64_t b = 0; b < PCA_FEATURES; ++b)
      cov[a][b] = cov[a][b] - (eig * vec[a] * vec[b]);
}

static fp64_t
project_sample_onto_component(uint64_t sample_idx, const fp64_t means[PCA_FEATURES], const fp64_t component[PCA_FEATURES])
{
  fp64_t score = 0.0;
  for (uint64_t k = 0; k < PCA_FEATURES; ++k)
  {
    fp64_t centered = data[sample_idx][k] - means[k];
    score = score + (centered * component[k]);
  }
  return score;
}

int
main(void)
{
  init_secret_dataset();

  fp64_t means[PCA_FEATURES];
  compute_means(means);

  fp64_t cov[PCA_FEATURES][PCA_FEATURES];
  compute_covariance(means, cov);

  fp64_t components[PCA_COMPONENTS][PCA_FEATURES] = {
    {1.0, 0.8, 0.6, 0.4, 0.2, 0.1},
    {0.1, 0.2, 0.4, 0.6, 0.8, 1.0}
  };

  fp64_t eigenvalues[PCA_COMPONENTS];

  for (uint64_t c = 0; c < PCA_COMPONENTS; ++c)
  {
    normalize_vec(components[c]);
    power_iteration(cov, components[c], &eigenvalues[c]);
    deflate_covariance(cov, components[c], eigenvalues[c]);
  }

  fp64_t total_variance = 0.0;
  for (uint64_t d = 0; d < PCA_FEATURES; ++d)
    total_variance = total_variance + cov[d][d];
  total_variance = total_variance + eigenvalues[0] + eigenvalues[1];

  fp64_t ratio0 = eigenvalues[0] / total_variance;
  fp64_t ratio1 = eigenvalues[1] / total_variance;

  fp64_t s0_c0 = project_sample_onto_component(0, means, components[0]);
  fp64_t s0_c1 = project_sample_onto_component(0, means, components[1]);
  fp64_t sN_c0 = project_sample_onto_component(PCA_SAMPLES - 1, means, components[0]);
  fp64_t sN_c1 = project_sample_onto_component(PCA_SAMPLES - 1, means, components[1]);

  libmin_printf("pca-analysis: encrypted PCA complete\n");
  libmin_printf("dataset shape: samples=%u features=%u retained_components=%u\n",
    (unsigned)PCA_SAMPLES, (unsigned)PCA_FEATURES, (unsigned)PCA_COMPONENTS);
  libmin_printf("eigenvalues: pc1=%lf pc2=%lf\n", eigenvalues[0], eigenvalues[1]);
  libmin_printf("explained variance ratios: pc1=%lf pc2=%lf cumulative=%lf\n",
    ratio0, ratio1, (ratio0 + ratio1));
  libmin_printf("projected sample[0]:  pc1=%lf pc2=%lf\n", s0_c0, s0_c1);
  libmin_printf("projected sample[%u]: pc1=%lf pc2=%lf\n", (unsigned)(PCA_SAMPLES - 1), sN_c0, sN_c1);

  libmin_success();
  return 0;
}
