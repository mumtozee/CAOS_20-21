/*float
dot_product (const float *A, const float *B)
{
  float ans = 0.0;
  for (int i = 0; i < 15; ++i)
    {
      ans += A[i] * B[i];
    }
  return ans;
}*/

#include <stdio.h>
float dot_product (int N, const float *A, const float *B);
#define SIZE 257

int
main ()
{
  float a[SIZE];
  float b[SIZE];
  for (int i = 0; i < SIZE; ++i)
    {
      a[i] = b[i] = 1.0;
    }
  printf ("%f\n", dot_product (SIZE, a, b));
  return 0;
}