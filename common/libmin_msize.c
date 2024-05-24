#include "libmin.h"
#include "libtarg.h"

/* return in-memory file size */
size_t
libmin_msize(MFILE *mfile)
{
  return mfile->data_sz;
}
