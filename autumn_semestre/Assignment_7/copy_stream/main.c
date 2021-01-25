#include <sys/syscall.h>
#include <sys/types.h>

long syscall (long number, ...);

void
my_exit (int code)
{
  syscall (SYS_exit, code);
}

ssize_t
write (int fd, const void *data, ssize_t size)
{
  return syscall (SYS_write, fd, data, size);
}

ssize_t
read (int fd, void *data, ssize_t size)
{
  return syscall (SYS_read, fd, data, size);
}

void
_start ()
{
  char c = 0;
  while (read (0, &c, 1))
    {
      write (1, &c, 1);
    }
  my_exit (0);
}