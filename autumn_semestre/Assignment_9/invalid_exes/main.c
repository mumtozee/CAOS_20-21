#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int
read_retry (int fd, char *data, ssize_t size)
{
  char *cdata = data;
  while (1)
    {
      ssize_t read_bytes = read (fd, cdata, size);
      if (read_bytes == 0)
        {
          return cdata - data;
        }
      if (read_bytes < 0)
        {
          if (errno == EAGAIN || errno == EINTR)
            {
              continue;
            }
          else
            {
              return -1;
            }
        }
      cdata += read_bytes;
      size -= read_bytes;
      if (size == 0)
        {
          return cdata - data;
        }
    }
}

int
IsELF (const char *meta_line)
{
  char elf_sign[4] = { 0x7F, 0x45, 0x4C, 0x46 };
  for (int i = 0; i < 4; ++i)
    {
      if (meta_line[i] != elf_sign[i])
        {
          return 0;
        }
    }
  return 1;
}

int
IsValidExecutable (const char *fname)
{ /* check for executablity first */
  char first_line[PATH_MAX + 2] = "";
  FILE *file = fopen (fname, "r");
  fgets (first_line, sizeof (first_line), file);
  int len = strlen (first_line);
  first_line[len - 1] = '\0';
  if (IsELF (first_line) == 1)
    {
      return 1;
    }
  else
    {
      return (access (first_line + 2, X_OK) == 0) ? 1 : 0;
    }
}

int
main (void)
{
  struct stat st;
  char filename[PATH_MAX] = "";
  while (fgets (filename, sizeof (filename), stdin))
    {
      int len = strlen (filename);
      filename[len - 1] = '\0';
      if (access (filename, X_OK) == 0)
        {
          if (IsValidExecutable (filename) == 0)
            {
              printf ("%s\n", filename);
            }
        }
    }
  return 0;
}