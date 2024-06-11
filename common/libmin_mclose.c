#include "libmin.h"
#include "libtarg.h"

/* close the in-memory file */
void
libmin_mclose(MFILE *mfile)
{
  /* reset the read pointer */
  mfile->rdptr = 0;
}
