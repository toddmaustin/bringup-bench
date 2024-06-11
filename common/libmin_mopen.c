#include "libmin.h"
#include "libtarg.h"

/* in-memory file I/O */

/* open an in-memory file */
void
libmin_mopen(MFILE *mfile, const char *mode)
{
  if (libmin_strcmp(mode, "r") != 0)
  {
    libmin_printf("ERROR: libmin only support file reads\n");
    libmin_fail(1);
  }

  /* reset the read pointer */
  mfile->rdptr = 0;
}
