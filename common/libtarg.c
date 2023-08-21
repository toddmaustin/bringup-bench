#include "libtarg.h"

#if defined(TARGET_HOST)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#elif defined(TARGET_SA)
/* standalone */
#include <stdio.h>
#include <stdlib.h>
#define MAX_SPIN  10000     /* make this larger for a real hardware platform */
#else /* undefined target */
#error Co-simulation platform not defined, define TARGET_HOST or a target-dependent definition.
#endif

#ifdef TARGET_SA
#define MAX_OUTBUF    (128*1024)
static uint8_t __outbuf[MAX_OUTBUF];
static uint32_t __outbuf_ptr = 0;
#endif /* TARGET_SA */

/* benchmark completed successfully */
void
libtarg_success(void)
{
#if defined(TARGET_HOST)
  exit(0);
#elif defined(TARGET_SA)
  uint64_t spincnt = 0;
  /* spin @ SPIN_SUCCESS_ADDR */
SPIN_SUCCESS_ADDR:
  spincnt++;
  if (spincnt < MAX_SPIN)
    goto SPIN_SUCCESS_ADDR;

  /* output any outbuf data */
  for (uint32_t i=0; i < __outbuf_ptr; i++)
    fputc(__outbuf[i], stdout);

  /* exit if we ever get here */
  exit(0);
#endif
}

/* benchmark completed with error CODE */
void
libtarg_fail(int code)
{
#ifdef TARGET_HOST
  exit(code);
#elif defined(TARGET_SA)
  uint64_t spincnt = 0;
  /* spin @ SUCCESS_SPIN_ADDR */
SPIN_FAIL_ADDR:
  spincnt++;
  if (spincnt < MAX_SPIN)
    goto SPIN_FAIL_ADDR;
  /* exit if we ever get here */
  exit(code);
#endif
}

/* output a single character, to wherever the target wants to send it... */
void
libtarg_putc(char c)
{
#if defined(TARGET_HOST)
  fputc(c, stdout);
#elif defined(TARGET_SA)
  /* add to outbuf pool */
  if (__outbuf_ptr >= MAX_OUTBUF)
    libtarg_fail(1);
  __outbuf[__outbuf_ptr++] = c;
#endif
}

#ifdef TARGET_SA
#define MAX_HEAP    (8*1024*1024)
static uint8_t __heap[MAX_HEAP];
static uint32_t __heap_ptr = 0;
#endif /* TARGET_SA */

/* get some memory */
void *
libtarg_sbrk(size_t inc)
{
#if defined(TARGET_HOST)
#if __clang__
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif /* __clang__ */
  return sbrk(inc);
#elif defined(TARGET_SA)
  uint8_t *ptr = &__heap[__heap_ptr];
  if (inc == 0)
    return ptr;
  
  __heap_ptr += inc;
  if (__heap_ptr >= MAX_HEAP)
    libtarg_fail(1);

  return ptr;
#endif
}

