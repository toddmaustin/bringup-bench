#include "libmin.h"

#define NUM_SAMPLES 25000  // samples

int
main(void)
{
  int count_inside_circle = 0;
  double x, y;

  // Seed the random number generator
  libmin_srand(42);

  libtarg_start_perf();
  for (int i = 0; i < NUM_SAMPLES; ++i)
  {
    // Generate random (x, y) point in [0, 1] × [0, 1]
    x = (double)libmin_rand() / RAND_MAX;
    y = (double)libmin_rand() / RAND_MAX;

    // Check if the point is inside the unit circle
    if (x*x + y*y <= 1.0)
      count_inside_circle++;
  }

  // Estimate Pi
  double pi_estimate = 4.0 * count_inside_circle / NUM_SAMPLES;
  libtarg_stop_perf();

  // Output result
  libmin_printf("Estimated Pi = %.8f\n", pi_estimate);

  libmin_success();
  return 0;
}
