#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int
main ()
{
  struct stat st;
  char filename[PATH_MAX] = "";
  off_t summ_size = 0;
  while (fgets (filename, sizeof (filename), stdin))
    {
      int len = strlen (filename);
      filename[len - 1] = '\0';
      lstat (filename, &st);
      summ_size += ((st.st_mode & S_IFMT) == S_IFREG) ? st.st_size : 0;
    }
  printf ("%ld\n", summ_size);
  return 0;
}