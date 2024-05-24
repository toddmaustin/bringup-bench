#include "libmin.h"
#include "libtarg.h"

#define MAXLN	1024
#define SPACE	"\t\n\f\r " 

int
libmin_sscanf(const char *buf, const char *fmt, ...)
{
  va_list vl;
  int i, j=0, ret=0;
  char *out_loc;
 	va_start(vl, fmt);

 	i = 0;
 	while (fmt && fmt[i] && buf[j])
 	{
    if (fmt[i] == '%') 
    {
      i++;
      switch (fmt[i]) 
      {
        case 'c': 
        {
	        *(char *)va_arg( vl, char*) = buf[j];
	        j++;
	        ret++;
	        break;
        }
        case 'd': 
        {
	         *(int *)va_arg(vl, int*) = libmin_strtol(&buf[j], &out_loc, 10);
	         j += (out_loc - &buf[j]);
	         ret++;
	         break;
        }
        case 'x': 
        {
	        *(int *)va_arg(vl, int*) = libmin_strtol(&buf[j], &out_loc, 16);
	        j += (out_loc - &buf[j]);
	        ret++;
	        break;
        }
      }
    } 
    else 
    {
      // buf[j] = fmt[i];
      j++;
    }
    i++;
  }
  va_end(vl);
  return ret;
}
