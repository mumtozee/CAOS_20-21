#include <stdio.h>
#include <string.h>

int
ProcessInput (const char *arg_str)
{
  int start_idx = 2;
  while (arg_str[start_idx] == '0')
    ++start_idx;

  int actual_length = strlen (arg_str + start_idx);

  return (actual_length == 0) ? 1 : (actual_length + 1) / 2;
}

int
main (int argc, char *argv[])
{
  for (int i = 1; i < argc; ++i)
    printf ("%d ", ProcessInput (argv[i]));
  return 0;
}