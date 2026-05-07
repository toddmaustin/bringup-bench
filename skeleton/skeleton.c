#include "libmin.h"

char *ptr = (char *)0ull-1;

int
main(void)
{
  libmin_printf("This is a test!, %d, %f...\n", 23, 44.4);

  // illegal insn
  // __asm__ volatile (".word 0x00000000");

  libmin_success();
  return 0;
}


