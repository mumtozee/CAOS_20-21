#include <stdio.h>

int N;
int *A;
int *B;
int *R;

void summ ();
void everyday795 (int X, int Y);

#define SIZE 3

int
main ()
{
  N = SIZE;
  int Ar[SIZE] = { 1, 2, 3 };
  int Br[SIZE] = { 1, 2, 3 };
  int Rr[SIZE];
  A = Ar;
  B = Br;
  R = Rr;
  summ ();
  for (int i = 0; i < N; ++i)
    {
      printf ("%d ", R[i]);
    }
  printf ("\n");
  everyday795 (2, 3);
  return 0;
}