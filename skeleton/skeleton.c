#include "libmin.h"

char *ptr = (char *)0ull-1;

int
main(void)
{
  libmin_printf("This is a test!, %d, %f...\n", 23, 44.4);

  // *ptr = *ptr + 1;
  // libmin_printf("This is string: %s\n", ptr);

  libmin_success();
  return 0;
}


