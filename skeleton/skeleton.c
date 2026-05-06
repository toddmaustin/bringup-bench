#include "libmin.h"

char *ptr = 0;

int
main(void)
{
  libmin_printf("This is a test!, %d, %f...\n", 23, 44.4);

  // *ptr = *ptr + 1;
  // libmin_printf("This is string: %s\n", ptr);

  libmin_success();
  return 0;
}


