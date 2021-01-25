#include <stdio.h>

int
main ()
{
  do
    {
      int c = fgetc (stdin);
      if (c == -1)
        break;
      if (c >= '0' && c <= '9')
        fputc (c, stdout);
    }
  while (1);
  return 0;
}