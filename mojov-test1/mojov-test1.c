#define SECRET_BUGS

__attribute__((__secret__)) long int x, xdst;
long int y;

int arr_public[128];
__attribute__((__secret__)) int arr_secret[128];

char *q, r, s;

int
bar(int x)
{
  return x+y;
}

#ifndef SECRET_BUGS
__attribute__((__secret__))
#endif /* SECRET_BUGS */
int
uxxe(int w)
{
  if (w != 22)
    return 1+(y-w);
  else
    return 2*((y>>1) - x);
}


int (*pbar)(int) = &bar;

__attribute__((__secret__)) int (*sbar)(int) = &bar;

void
foo(void)
{
  int *p = arr_public;

#ifdef SECRET_BUGS
  if (x)
    x++;
#endif /* SECRET_BUGS */

  switch (y)
  {
  case 1:
    x++;
    break;

  case 4:
    x--;
    break;
  }

#ifdef SECRET_BUGS
  switch (y+(1-(43*x)))
  {
  case 1:
    x++;
    break;

  case 4:
    x--;
    break;
  default:
    y++;
    break;
  }
#endif /* SECRET_BUGS */

#ifdef SECRET_BUGS
  switch (x = y+(1-(43*y)))
  {
  case 1:
    x++;
    break;

  case 4:
    x--;
    break;
  default:
    y++;
    break;
  }
#endif /* SECRET_BUGS */

  if (y)
    y++;

#ifdef SECRET_BUGS
  while (x)
    x++;
#endif /* SECRET_BUGS */

#ifdef SECRET_BUGS
  while ((x+y) < 22)
    x++;
#endif /* SECRET_BUGS */

#ifdef SECRET_BUGS
  while ((x = ((y + 1) < 53)))
    y++;
#endif /* SECRET_BUGS */

  while ((y + 1) < 53)
    y++;

#ifdef SECRET_BUGS
  do {
    x++;
  } while (x);
#endif /* SECRET_BUGS */

#ifdef SECRET_BUGS
  do {
    x++;
  } while ((x+y) < 22);
#endif /* SECRET_BUGS */

#ifdef SECRET_BUGS
  do {
    y++;
  } while ((x = ((y + 1) < 53)));
#endif /* SECRET_BUGS */

  do {
    y++;
  } while ((y + 1) < 53);

#ifdef SECRET_BUGS
  y = (x < 0) ? y+1 : y-1;
  y = (1-x < 0) ? y+1 : y-1;
  y = (1-arr_secret[y] < 0) ? y+1 : y-1;
#endif /* SECRET_BUGS */

  y = (y < 0) ? y+1 : y-1;

  y = (y < 0) ? y+1 : x-1;

  y = (*pbar)(42);

  x = bar(42);

#ifdef SECRET_BUGS
  x = (*sbar)(42);
#endif /* SECRET_BUGS */

  x = arr_secret[y];

#ifdef SECRET_BUGS
  y = arr_secret[x];
#endif /* SECRET_BUGS */

#ifdef SECRET_BUGS
  y = arr_secret[y + (1-x)];
#endif /* SECRET_BUGS */

  x = arr_public[y];

#ifdef SECRET_BUGS
  y = arr_public[x];
#endif /* SECRET_BUGS */

#ifdef SECRET_BUGS
  y = arr_public[y + (1-x)];
#endif /* SECRET_BUGS */

  p += y;

  p = p + y;

#ifdef SECRET_BUGS
  p += x;
#endif /* SECRET_BUGS */

#ifdef SECRET_BUGS
  p = p + x;
#endif /* SECRET_BUGS */

#ifdef SECRET_BUGS
  p = p + (y - (1 + x));
#endif /* SECRET_BUGS */

  // cast tests
  q = (char *)y;

#ifdef SECRET_BUGS
  r = (char *)x;
#endif /* SECRET_BUGS */

#ifdef SECRET_BUGS
  s = (char *)(y - (1 + x));
#endif /* SECRET_BUGS */

#ifdef SECRET_BUGS
  y = x;
#endif /* SECRET_BUGS */

#ifdef SECRET_BUGS
  y = 1 - (y/2 + x);
#endif /* SECRET_BUGS */

  xdst = x;

  xdst = 1 - (y/2 + x);


}

int
main(void)
{
  foo();
  return 0;
}
