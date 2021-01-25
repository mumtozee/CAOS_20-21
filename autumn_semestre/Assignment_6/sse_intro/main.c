#include <stdio.h>

#define SIZE 2

double calc (double A, double B, double C, int D);
void vector_sum (int N, const float *a_, const float *b_, float *c_);

int
main (void)
{
  float __attribute__ ((aligned (32))) A[4 * SIZE]
      = { 0.0, 1.2, 3.14, 4.234, 5.2, 6.1234, 5.5, 9.21 };
  float __attribute__ ((aligned (32))) B[4 * SIZE]
      = { 0.0, 1.2, 3.14, 4.234, 5.2, 6.1234, 5.5, 9.21 };
  float __attribute__ ((aligned (32))) C[4 * SIZE];
  double a = 2;
  double b = 6;
  double c = 1;
  double d = 3;
  printf ("%lf\n", calc (a, b, c, d));
  vector_sum (4 * SIZE, A, B, C);
  for (int i = 0; i < 4 * SIZE; ++i)
    {
      printf ("%f ", C[i]);
    }
  printf ("\n");
  return 0;
}