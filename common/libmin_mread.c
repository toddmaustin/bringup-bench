#include "libmin.h"
#include "libtarg.h"

/* read a buffer from the in-memory file */
size_t
libmin_mread(void *_ptr, size_t size, MFILE *mfile)
{
  if (libmin_meof(mfile))
    return 0;

  char *ptr = _ptr;
  size_t cnt = 0;
  while (mfile->rdptr < mfile->data_sz && cnt < size && !libmin_meof(mfile))
  {
    *ptr++ = mfile->data[mfile->rdptr++];
    cnt++;
  }
  return cnt;
}
