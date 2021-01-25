#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_WORD_SIZE 4096

int
main ()
{
  pid_t fpid = fork ();
  if (fpid == 0)
    {
      for (uint8_t i = 0; i < 255; ++i)
        {
          char word[MAX_WORD_SIZE + 1] = "";
          int read = scanf ("%s", word);
          pid_t pid = fork ();
          if (pid == 0)
            {
              if (read == EOF)
                {
                  return -1;
                }
              continue;
            }
          if (pid > 0)
            {
              int status = 0;
              waitpid (pid, &status, 0);
              return WEXITSTATUS (status) + 1;
            }
        }
    }
  else
    {
      int status = 0;
      waitpid (fpid, &status, 0);
      printf ("%d\n", WEXITSTATUS (status));
    }
  return 0;
}