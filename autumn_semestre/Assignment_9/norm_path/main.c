#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern void normalize_path (char *path);

int
main (void)
{
  char path[256] = "";
  fgets (path, sizeof (path), stdin);
  normalize_path (path);
  printf ("%s\n", path);
  return 0;
}