#include "libmin.h"
#include "libtarg.h"

/* at end of file */
int
libmin_meof(MFILE *mfile)
{
  return mfile->rdptr >= mfile->data_sz;
}
