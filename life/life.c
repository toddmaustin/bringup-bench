#include "libmin.h"

void init(void);
void draw(void);
void process(void);
void processMutate(int x, int y);
int getNumNeigbors(int x, int y);
void clrscr(void);
void flip(void);
char getLeft(int x, int y);
char getRight(int x, int y);
char getUp(int x, int y);
char getDown(int x, int y);
char getUpLeft(int x, int y);
char getUpRight(int x, int y);
char getDownLeft(int x, int y);
char getDownRight(int x, int y);
void sleep(unsigned int mseconds);

#define MS_DELAY	1
#define GRID_WIDTH	70
#define GRID_HEIGHT	22

int NUM_BLOCKS = 2;
char LIVE = '*';
char DEAD = ' ';
char BLOCKS[2] = { ' ', '*' };

unsigned int evolution = 1;

char *gridRoots[GRID_WIDTH];
char *gridTmpRoots[GRID_WIDTH];

char **grid = gridRoots;
char **gridTmp = gridTmpRoots;

char gridVals[GRID_WIDTH][GRID_HEIGHT];
char gridTmpVals[GRID_WIDTH][GRID_HEIGHT];

int
main(void)
{
  int iters = 0;

  init();
  int running = TRUE;
  while (running) {
    draw();
    //sleep(500);
    process();
    iters++;
    if (iters == 80)
      running = FALSE;
  }

  libmin_success();
  return 0;
}


void
init(void)
{ 
  libmin_srand(1001);
  for (int x = 0; x < GRID_WIDTH; ++x)
    {
      grid[x] = gridVals[x];
      gridTmp[x] = gridTmpVals[x];
      for (int y = 0; y < GRID_HEIGHT; ++y)
        {
          grid[x][y] = BLOCKS[libmin_rand() % NUM_BLOCKS];
          gridTmp[x][y] = ' ';
        }
    }

  // clear the screen
  libmin_printf("\x1b[2J");
}

void
draw(void)
{
  // go to home position on screen
  libmin_printf("\x1b[H");

  libmin_printf("CONWAY'S Game of Life\n\nEvolution #%d\n\n", evolution++);
  for (int y = 0; y < GRID_HEIGHT; ++y)
    {
      for (int x = 0; x < GRID_WIDTH; ++x)
        libmin_printf("%c", grid[x][y]);
      libmin_printf("\n");
    }
}

void
process(void)
{
  for (int y = 0; y < GRID_HEIGHT; ++y)
    {
      for (int x = 0; x < GRID_WIDTH; ++x)
        {
          int neighbors = getNumNeigbors(x, y);
          if (grid[x][y] == LIVE)
            {
              //1. Any live cell with fewer than two live neighbors dies, as if caused by underpopulation.
              //2. Any live cell with more than three live neighbors dies, as if by overcrowding.
              if (neighbors < 2 || neighbors > 3)
                gridTmp[x][y] = DEAD;
              else
                gridTmp[x][y] = LIVE;
              //3. Any live cell with two or three live neighbors lives on to the next generation.
            }
          else
            {
              //4. Any dead cell with exactly three live neighbors becomes a live cell.
              if (neighbors == 3)
                gridTmp[x][y] = LIVE;
              else
                gridTmp[x][y] = DEAD;
            }
          processMutate(x, y);
        }
    }
    flip();
}

int
getNumNeigbors(int x, int y)
{
  int i = 0;

  if (getLeft(x, y) == LIVE)
    i++;
  if (getRight(x, y) == LIVE)
    i++;
  if (getUp(x, y) == LIVE)
    i++;
  if (getDown(x, y) == LIVE)
    i++;
  if (getUpLeft(x, y) == LIVE)
    i++;
  if (getUpRight(x, y) == LIVE)
    i++;
  if (getDownLeft(x, y) == LIVE)
    i++;
  if (getDownRight(x, y) == LIVE)
    i++;

  return i;
}


void
processMutate(int x, int y)
{
  if (libmin_rand() % 3000 > 2927)
    {
      if (gridTmp[x][y] == DEAD)
        ; // gridTmp[x][y] = LIVE;
    }
}

void
flip(void)
{
  char **tmp = grid;
  grid = gridTmp;
  gridTmp = tmp;
}


char
getLeft(int x, int y)
{
  if (x == 0)
  {
    return DEAD;
  }

  char retval = grid[x - 1][y];

  return retval;
}

char
getRight(int x, int y)
{
  if (x == GRID_WIDTH - 1)
  {
    return DEAD;
  }

  char retval = grid[x + 1][y];

  return retval;
}

char
getUp(int x, int y)
{
  if (y == 0)
  {
    return DEAD;
  }

  char retval = grid[x][y - 1];

  return retval;
}

char
getDown(int x, int y)
{
  if (y == GRID_HEIGHT - 1)
  {
    return DEAD;
  }

  char retval = grid[x][y + 1];

  return retval;
}

char
getUpLeft(int x, int y)
{
  if (x == 0 || y == 0)
  {
    return DEAD;
  }

  char retval = grid[x - 1][y - 1];

  return retval;
}

char
getUpRight(int x, int y)
{
  if (x == GRID_WIDTH - 1 || y == 0)
  {
    return DEAD;
  }
 
  char retval = grid[x + 1][y - 1];

  return retval;
}

char
getDownLeft(int x, int y)
 {
  if (y == GRID_HEIGHT - 1 || x == 0)
  {
    return DEAD;
  }
 
  char retval = grid[x - 1][y + 1];

  return retval;
}

char
getDownRight(int x, int y)
{
  if (y == GRID_HEIGHT - 1 || x == GRID_WIDTH - 1 )
  {
    return DEAD;
  }
 
  char retval = grid[x + 1][y + 1];

  return retval;
}

volatile int x;

void
sleep(unsigned int mseconds)
{
  x = 0;
  while (x < MS_DELAY)
    x++;
}

