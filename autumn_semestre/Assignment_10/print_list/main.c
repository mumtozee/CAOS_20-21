#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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

typedef struct
{
  int val_;
  uint32_t next_;
} Item;

void
PrintList (const Item *list)
{
  ssize_t i = 0;
  do
    {
      printf ("%d ", list[i].val_);
      i = list[i].next_ / 8;
    }
  while (i != 0);
  printf ("\n");
}

int
main (int argc, char *argv[])
{
  const char *fname = argv[1];
  int fd = open (fname, O_RDONLY);
  ssize_t fsize = GetFileSize (fd);
  if (fsize / 8 > 0)
    {
      char *data = mmap (NULL, fsize, PROT_READ, MAP_PRIVATE, fd, 0);
      Item *list = (Item *)data;
      PrintList (list);
      munmap (data, fsize);
    }
  close (fd);
  return 0;
}