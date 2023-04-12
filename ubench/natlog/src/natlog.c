#include "libcosim.h"

/* calculate e=2.718..., using an interative approximation */

int
newmain(void)
{
  /* STEPS is usually a very large number eg 10000000 */
  int steps = 10000;
  double x, y;

  y = 1.0 + 1.0/steps;
  x = 1.0;

  for(; steps > 0; steps--)
    x *= y;

  cosim_printf("natlog: e=%f\n", x);
  return 0;
}

