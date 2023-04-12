#include <stdlib.h>
#include "libcosim.h"

int
newmain(void)
{
  cosim_printf("This is a test!, %d, %f...\n", 23, 44.4);

  /* cosim_fail(2); */

  return 0;
}


