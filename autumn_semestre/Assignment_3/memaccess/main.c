#include <stdio.h>

int summ (int x0, int N, int *X);

#define SIZE 100

int
main ()
{
  int arr[SIZE] = { 0 };
  for (int i = 0; i < SIZE; ++i)
    arr[i] = 2 * i + 1;
  printf ("%d\n", summ (0, SIZE, arr));
  return 0;
}