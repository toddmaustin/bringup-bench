#include "libmin.h"

#define M_E 2.71828182845904523536
#define MAX_STEPS 10000l

int
main(void)
{
  // initialize the pseudo-RNG
  libmin_srand(42);

  uint64_t steps = MAX_STEPS; /* STEPS is usually a very large number eg 10000000 */
  double e, term;

  // comupute "e" via compound interest limit
  e = 1.0;
  term = 1.0 + (1.0/steps);

  for(; steps > 0; steps--)
    e = e * term;

  libmin_printf("INFO: e via compound interest after %lu steps == %.16lf (%.16lf error).\n",
                MAX_STEPS, e, M_E - e);

  // compute "e" via the infinite series method
  e = 1.0;              // Starts at 1/0! = 1
  term = 1.0;   // The current fraction being added (initially 1/1!)
  int k = 1;
  uint64_t cnt = 0;

  // keep adding terms until the fractions become smaller than 
  // what a double can accurately hold.
  for (k=1; k <= 64; k++)
  {
    if (term > 1e-17)
    {
      term = term / k; // Generates the next term: 1/k!
      e = e + term; // Adds 1/k! to the total
      cnt = cnt + 1;
    }
  }

  // print the result with maximum double precision
  libmin_printf("INFO: e via infinite series after %u steps == %.16lf (%.16lf error).\n",
                cnt, e, M_E - e);

  libmin_success();
  return 0;
}
