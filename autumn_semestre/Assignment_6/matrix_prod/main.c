#include <smmintrin.h>
#include <stdio.h>
#include <stdlib.h>

float
dot_product (int N, const float *vec_a, const float *vec_b)
{
  int remainder = N % 4;
  N -= remainder;
  float ans = 0.0;
  int i = 0;
  for (; i < N; i += 4)
    {
      __m128 smm0 = _mm_loadu_ps (vec_a + i);
      __m128 smm1 = _mm_loadu_ps (vec_b + i);
      __m128 smm2 = _mm_dp_ps (smm0, smm1, 0xF1); // standard dot product
      float tmp_result = 0.0;
      _mm_store_ss (&tmp_result, smm2);
      ans += tmp_result;
    }
  N += remainder;
  for (int j = i; j < N; ++j)
    {
      ans += vec_a[j] * vec_b[j];
    }
  return ans;
}

void
multiply (const float *A, const float *B, float *C, int row_size, int col_size)
{
  for (int i = 0; i < row_size; ++i)
    {
      for (int j = 0; j < row_size; ++j)
        {
          C[i * row_size + j]
              = dot_product (col_size, A + i * col_size, B + j * col_size);
        }
    }
}

int
main ()
{
  int row_size = 0;
  int col_size = 0;
  scanf ("%d", &row_size);
  scanf ("%d", &col_size);
  float *A = (float *)malloc (row_size * col_size * sizeof (float));
  float *B = (float *)malloc (row_size * col_size * sizeof (float));
  float *C = (float *)malloc (row_size * row_size * sizeof (float));

  for (int row_i = 0; row_i < row_size; ++row_i)
    {
      for (int col_i = 0; col_i < col_size; ++col_i)
        {
          float tmp = 0.0;
          scanf ("%f", &tmp);
          A[row_i * col_size + col_i] = tmp;
        }
    }

  for (int row_i = 0; row_i < col_size; ++row_i)
    {
      for (int col_i = 0; col_i < row_size; ++col_i)
        {
          float tmp = 0.0;
          scanf ("%f", &tmp);
          B[col_i * col_size + row_i] = tmp;
        }
    }

  multiply (A, B, C, row_size, col_size);

  for (int i = 0; i < row_size; ++i)
    {
      for (int j = 0; j < row_size; ++j)
        {
          printf ("%.4f ", C[i * row_size + j]);
        }
      printf ("\n");
    }

  free ((void *)A);
  free ((void *)B);
  free ((void *)C);
  return 0;
}
