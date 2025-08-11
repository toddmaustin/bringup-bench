#include "libmin.h"
#include "speech.h"

#define BUFFER_LENGTH 511

struct _MFILE speech = { "speech.txt", __speech_sz, __speech, 0 };

int
main(void)
{
  int ac = 3; char *av[] = { "simple-grep", "speech.txt", "the" };
  char lineBuffer[BUFFER_LENGTH+1];
  MFILE *fp = &speech;
  int count = 0;

  if (ac < 3)
  {
    libmin_printf("A string pattern and a file name are required\n" );
    return 1;
  }

  libmin_mopen(&speech, "r");
  if (!fp)
  {
    libmin_printf("Error - unable to open %s\n", av[1]);
    return 2;
  }

  libtarg_start_perf();
  while (libmin_mgets(lineBuffer, BUFFER_LENGTH, fp))
  {
    // libmin_printf("%s\n", lineBuffer);
    if (libmin_strstr(lineBuffer, av[2]))
    {
      ++count;
    }
  }
  libtarg_stop_perf();

  // Reopen and print matching lines
  libmin_mclose(fp);
  libmin_mopen(&speech, "r");
  while (libmin_mgets(lineBuffer, BUFFER_LENGTH, fp))
  {
    if (libmin_strstr(lineBuffer, av[2]))
    {
      libmin_printf("%s", lineBuffer);
    }
  }
  libmin_mclose(fp);

  libmin_printf("INFO: found (at least) %d occurrences of the word `%s'.\n", count, av[2]);

  libmin_success();
  return 0;
}
