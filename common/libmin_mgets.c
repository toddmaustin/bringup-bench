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

// this function will get a line from a FILE *
// returns the amount of chars read
// you should pass it an NULL char *
int
libmin_getline(char **line, size_t *bufflen, MFILE *fp)
{
  // if you allocated something to line it will return error code -1
  if (*line == NULL)
  {
    *bufflen = 80;
    *line = libmin_malloc(*bufflen * sizeof(char));
  }

  int i = 0;
  while(1)
  {
    if (i == *bufflen - 1) // if buffer is full resize
    {
      char * tempbuff = libmin_malloc(2 * (*bufflen) * sizeof(char)); // we create a larger buffer
      libmin_strncpy(tempbuff, *line, *bufflen); // we copy the old buffer to the temp
      libmin_free(*line); // we free the old buffer
      *line = tempbuff; // we save the pointer to the new buffer to buff
      tempbuff = NULL; // this is to prevent accidentally deleting buff
      *bufflen = *bufflen * 2; // we update the size of the buffer
    }
    char c = libmin_mgetc(fp); // gets a char from file descriptor
    if (c == '\n' || c == EOF)
    {
      (*line)[i] = '\0'; // make buff a c string
      break;
    }
    else if (c == '\b' && i > 0)
      i = i - 2; // if back space remove element from buffer
    else
      (*line)[i] = c; // if any other char add it
	  i++;
  }

  return (i <= 0) ? -1 : i; // returns the size of the line read
}

