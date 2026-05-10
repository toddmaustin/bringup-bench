#include "libmin.h"

double 
rad2deg(double rad)
{
      return ((double)180.0 * rad / (M_PI));
}

double
deg2rad(double deg)
{
      return ((double)M_PI * deg / 180.0);
}

int
main(void)
{
  // initialize the pseudo-RNG
  libmin_srand(42);

  {
    // Stopwatch s("VIP_Bench Runtime");

    for (double x = 0.0; x <= 360.0; x += 1.0)
      libmin_printf("INFO: deg2rad(%.5lf) == %.5lf\n", x, deg2rad((double)x));

    for (double x = 0.0; x <= (2 * M_PI + 1e-6); x += (M_PI / 180))
      libmin_printf("INFO: rad2deg(%.5lf) == %.5lf\n", x, rad2deg((double)x));
  }

  libmin_success();
  return 0;
}
