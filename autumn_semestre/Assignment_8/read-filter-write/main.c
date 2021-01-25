#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
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

void
FilterData (const char *inp_data, char *digits, char *other, ssize_t data_size,
            ssize_t *digit_size, ssize_t *other_size)
{
  ssize_t j = 0; // digits idx
  ssize_t k = 0; // other idx
  for (ssize_t i = 0; i < data_size; ++i)
    {
      if (isdigit (inp_data[i]))
        {
          digits[j] = inp_data[i];
          ++j;
        }
      else
        {
          other[k] = inp_data[i];
          ++k;
        }
    }
  *digit_size = j;
  *other_size = k;
}

int
FinishAndExit (int d1, int d2, int d3, int code)
{
  if (d1 != -1)
    {
      close (d1);
    }
  if (d2 != -1)
    {
      close (d2);
    }
  if (d3 != -1)
    {
      close (d3);
    }
  return code;
}

#define MAX_BUFFSIZE (1 << 20)

int
main (int argc, char *argv[])
{
  char *input_file = argv[1];
  char *out_file_digits = argv[2];
  char *out_file_other = argv[3];

  int in_d = open (input_file, O_RDONLY);
  if (in_d == -1)
    {
      return 1;
    }
  int out_dig_d = open (out_file_digits, O_WRONLY | O_CREAT,
                        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
  int out_oth_d = open (out_file_other, O_WRONLY | O_CREAT,
                        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
  if (out_dig_d == -1 || out_oth_d == -1)
    {
      close (in_d);
      return 2;
    }
  char *main_buf = malloc (MAX_BUFFSIZE);
  char *digits = malloc (MAX_BUFFSIZE);
  char *other = malloc (MAX_BUFFSIZE);
  ssize_t read = read_retry (in_d, main_buf, MAX_BUFFSIZE);
  while (read != 0)
    {
      ssize_t digits_sz = 0;
      ssize_t other_sz = 0;
      FilterData (main_buf, digits, other, read, &digits_sz, &other_sz);
      write_retry (out_dig_d, digits, digits_sz);
      write_retry (out_oth_d, other, other_sz);
      read = read_retry (in_d, main_buf, MAX_BUFFSIZE);
    }
  free (main_buf);
  free (digits);
  free (other);
  return FinishAndExit (in_d, out_dig_d, out_oth_d, 0);
}