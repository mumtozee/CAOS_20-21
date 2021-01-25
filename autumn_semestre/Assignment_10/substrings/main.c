#define _GNU_SOURCE
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

ssize_t
GetFileSize (int fd)
{
  struct stat st;
  fstat (fd, &st);
  return st.st_size;
}

int
main (int argc, char *argv[])
{
  const char *fname = argv[1];
  const char *pattern = argv[2];
  ssize_t pattern_sz = strlen (pattern);
  int fd = open (fname, O_RDONLY);
  ssize_t fsize = GetFileSize (fd);
  char *text = mmap (NULL, fsize, PROT_READ, MAP_PRIVATE, fd, 0);

  char *found = memmem (text, fsize, pattern, pattern_sz);
  while (found != NULL)
    {
      ssize_t i = found - text;
      printf ("%ld ", i);
      found = memmem (found + pattern_sz, fsize - i - pattern_sz, pattern,
                      pattern_sz);
    }

  munmap (text, fsize);
  close (fd);
  return 0;
}