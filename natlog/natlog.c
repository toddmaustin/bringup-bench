#include "libmin.h"

/* calculate e=2.718..., using an interative approximation */

int
main(void)
{
  /* STEPS is usually a very large number eg 10000000 */
  int steps = 100000;
  double x, y;

  y = 1.0 + 1.0/steps;
  x = 1.0;

  libtarg_start_perf();
  for(; steps > 0; steps--)
    x *= y;
  libtarg_stop_perf();

  libmin_printf("natlog: e=%f\n", x);

  libmin_success();
  return 0;
}

