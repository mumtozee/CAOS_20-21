#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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
write_retry (int fd, const char *data, ssize_t size)
{
  const char *cdata = data;
  while (1)
    {
      ssize_t write_bytes = write (fd, cdata, size);
      if (write_bytes == 0)
        {
          return cdata - data;
        }
      if (write_bytes < 0)
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
      cdata += write_bytes;
      size -= write_bytes;
      if (size == 0)
        {
          return cdata - data;
        }
    }
}

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
PrintList (int fd)
{
  uint32_t i = 0;
  Item tmp;
  do
    {
      lseek (fd, i, SEEK_SET);
      read_retry (fd, (char *)&tmp, sizeof (Item));
      printf ("%d ", tmp.val_);
      i = tmp.next_;
    }
  while (i != 0);
  printf ("\n");
}

int
main (int argc, char *argv[])
{
  char *filename = argv[1];
  int fd = open (filename, O_RDONLY);
  ssize_t filesize = GetFileSize (fd);
  if (filesize / 8 > 0)
    {
      PrintList (fd);
    }

  close (fd);
  return 0;
}