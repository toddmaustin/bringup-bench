//#define SECRET_BUGS

__attribute__((__secret__)) int x;
int y;

void foo(void)
{
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
#endif /* SECRET_BUGS */

  y = (y < 0) ? y+1 : y-1;
  y = (y < 0) ? y+1 : x-1;

}

int
main(void)
{
  foo();
  return 0;
}
