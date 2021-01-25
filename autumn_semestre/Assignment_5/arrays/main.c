#include <stdio.h>

void summ (int N, const int *A, const int *B, int *R);

#define SIZE 0

int
main ()
{
  int N = SIZE;
  int A[SIZE] = {};
  int B[SIZE] = {};
  int R[SIZE];
  summ (N, A, B, R);
  for (int i = 0; i < N; ++i)
    {
      printf ("%d ", R[i]);
    }
  printf ("\n");
  return 0;
}