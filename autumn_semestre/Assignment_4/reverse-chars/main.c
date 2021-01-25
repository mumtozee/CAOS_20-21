#include <stdio.h>
#include <stdlib.h>

int
main ()
{
  int max_length = 125;
  int cur_size = 0;
  int *str = malloc (max_length);
  while (str != NULL)
    {
      int c = fgetc (stdin);
      if (c == EOF)
        break;
      str[cur_size++] = c;
      if (cur_size == max_length)
        {
          cur_size += max_length;
          str = realloc (str, cur_size);
        }
    }
  for (int i = cur_size - 1; i >= 0; --i)
    {
      fputc (str[i], stdout);
    }
  free (str);
  return 0;
}