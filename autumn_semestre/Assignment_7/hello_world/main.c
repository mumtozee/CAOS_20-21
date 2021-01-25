#include <sys/syscall.h>

long syscall (long number, ...);

long
__exit (int status)
{
  syscall (SYS_exit, status);
}

void
_start ()
{
  const char hello[] = "Hello, World!";
  syscall (SYS_write, 1, hello, sizeof (hello) - 1);
  __exit (0);
}