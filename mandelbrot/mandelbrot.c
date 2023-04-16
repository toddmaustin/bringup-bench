/**  mandel.c   by Eric R. Weeks   written 9-28-96
 **  weeks@dept.physics.upenn.edu
 **  http://dept.physics.upenn.edu/~weeks/
 **  
 **  This program is public domain, but this header must be left intact
 **  and unchanged.
 **  
 **  to compile:  cc -o mand mandel.c
 ** 
 **/

#include "libmin.h"

void
color(int red, int green, int blue)
{
  libmin_printf("<%d,%d,%d>\n", red, green, blue);
}

int
main(void)
{
  double x, xx, y, cx, cy;
  int iteration, hx, hy;
  int itermax = 100;		/* how many iterations to do	*/
  double magnify = 1.0;		/* no magnification		*/
  int hxres = 78;		/* horizonal resolution		*/
  int hyres = 48;		/* vertical resolution		*/

  /* header for PPM output */
  libmin_printf("** Mandelbrot ASCII image\n");
  libmin_printf("** xres: %d, yres: %d\n", hxres, hyres);

  for (hy=1; hy <= hyres; hy++)
    {
      for (hx=1; hx <= hxres; hx++)
	{
	  cx = (((float)hx)/((float)hxres)-0.5)/magnify*3.0-0.7;
	  cy = (((float)hy)/((float)hyres)-0.5)/magnify*3.0;
	  x = 0.0; y = 0.0;

	  for (iteration=1; iteration < itermax; iteration++)
	    {
	      xx = x*x-y*y+cx;
	      y = 2.0*x*y+cy;
	      x = xx;
	      if (x*x+y*y > 100.0)
		iteration = 999999;
	    }
	  if (iteration < 99999)
	    libmin_printf(" ");
	  else
	    libmin_printf("0");
	}
      libmin_printf("\n");
    }

  libmin_success();
  return 0;
}
