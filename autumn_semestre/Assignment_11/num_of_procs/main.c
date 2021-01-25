#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
main ()
{
  size_t count = 1;
  while (1)
    {
      int pid = fork ();
      fflush (stdout);
      if (pid < 0)
        {
          printf ("%ld\n", count);
          return 0;
        }
      if (pid != 0)
        {
          int status = 0;
          waitpid (pid, &status, 0);
          break;
        }
      ++count;
    }
  return 0;
}