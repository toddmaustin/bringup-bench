#include "libmin.h"
#include "nonlinear-nn-inputs.h"

// VIP type mapping for this port:
//   VIP_ENCCHAR   -> int8_t
//   VIP_ENCBOOL   -> uint64_t
//   VIP_ENCUINT64 -> uint64_t
//   VIP_ENCFLOAT  -> float
//   VIP_ENCDOUBLE -> double

// LeakyReLU uses a small slope for negative values.
static const double kLeakyAlpha = 0.01;

// ReLU(x) = max(x, 0).
static inline double
relu(double x)
{
  return (x > (double)0.0) ? x : (double)0.0;
}

// LeakyReLU(x) = x when x>0, else alpha*x.
static inline double
leaky_relu(double x, double alpha)
{
  return (x > (double)0.0) ? x : (alpha * x);
}

int
main(void)
{
  // Initialize the pseudo-RNG.
  libmin_srand(42);

  // Encrypted globals cannot currently be initialized statically in Mojo-V,
  // so populate encrypted arrays and constants explicitly in main().
  double alpha = (double)kLeakyAlpha;
  double inputs[NONLINEAR_NN_INPUT_COUNT];
  for (unsigned i = 0; i < NONLINEAR_NN_INPUT_COUNT; i++)
    inputs[i] = (double)nonlinear_nn_inputs_plain[i];

  for (unsigned i = 0; i < NONLINEAR_NN_INPUT_COUNT; i++)
  {
    double x = inputs[i];
    libmin_printf("INFO: ReLU(%.5lf) == %.5lf\n",
                  nonlinear_nn_inputs_plain[i],
                  relu(x));
    libmin_printf("INFO: LeakyReLU(%.5lf) == %.5lf\n",
                  nonlinear_nn_inputs_plain[i],
                  leaky_relu(x, alpha));
  }

  libmin_success();
  return 0;
}
