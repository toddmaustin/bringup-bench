#include "libmin.h"
#include "libtarg.h"

/* read a character from the in-memory file */
int
libmin_mgetc(MFILE *mfile)
{
  if (mfile->rdptr >= mfile->data_sz)
    return EOF;
  return mfile->data[mfile->rdptr++];
}
