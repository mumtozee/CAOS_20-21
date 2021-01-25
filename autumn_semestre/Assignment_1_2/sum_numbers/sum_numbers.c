#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int
main (int argc, char *argv[])
{
  double x;
  unsigned int y;
  const char *z = argv[1];
  scanf ("%lf", &x);
  scanf ("%x", &y);
  int64_t zbs = strtol (z, NULL, 27);
  printf ("%.3lf", x + y + zbs);
  return 0;
}