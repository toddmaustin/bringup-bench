#include "libmin.h"
#include "libtarg.h"

/* get a string from the in-memory file */
char *
libmin_mgets(char *s, size_t size, MFILE *mfile)
{
  if (libmin_meof(mfile))
    return NULL;

  char *p = s;
  size_t cnt;
  
  for (cnt=0; mfile->data[mfile->rdptr] != '\n' && cnt < (size-1) && !libmin_meof(mfile); cnt++)
    *p++ = mfile->data[mfile->rdptr++];

  if (mfile->data[mfile->rdptr] == '\n')
    mfile->rdptr++;

  *p = '\0';

  return s;
}
