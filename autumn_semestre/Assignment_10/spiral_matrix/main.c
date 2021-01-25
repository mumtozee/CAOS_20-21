#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void
FillSpiralMatrix (int64_t *spiral, int64_t rows, int64_t cols)
{
  int64_t val = 1;
  int64_t rown = 0;
  int64_t coln = 0;
  int64_t m = rows;
  int64_t n = cols;
  while (rown < m && coln < n)
    {
      for (int64_t i = coln; i < n; ++i)
        {
          spiral[rown * cols + i] = val++;
        }
      ++rown;
      for (int64_t i = rown; i < m; ++i)
        {
          spiral[i * cols + n - 1] = val++;
        }
      --n;
      if (rown < m)
        {
          for (int64_t i = n - 1; i >= coln; --i)
            {
              spiral[(m - 1) * cols + i] = val++;
            }
          --m;
        }
      if (coln < n)
        {
          for (int64_t i = m - 1; i >= rown; --i)
            {
              spiral[i * cols + coln] = val++;
            }
          ++coln;
        }
    }
}

void
PrintMatrix (char *data, const int64_t *matrix, int64_t N, int64_t W,
             const char *format, int64_t format_size)
{
  for (int64_t i = 0; i < N; ++i)
    {
      for (int64_t j = 0; j < N; ++j)
        {
          char *lltos = malloc (W + 1);
          lltos[0] = '\0';
          snprintf (lltos, W + 1, "%ld", matrix[i * N + j]);
          int64_t idx = i * ((N * W) + 1) + j * W;
          snprintf (data + idx, W + 1, format, lltos);
          free (lltos);
        }
      data[i * ((N * W) + 1) + N * W] = '\n';
    }
}

int
main (int argc, char *argv[])
{
  const char *fname = argv[1];
  int64_t N = strtoll (argv[2], NULL, 10);
  int64_t W = strtoll (argv[3], NULL, 10);
  int64_t format_size = strlen (argv[3]);
  char *format = malloc (2 * format_size + 4);
  format[0] = '\0';
  strcat (format, "%");
  strcat (format, argv[3]);
  strcat (format, ".");
  strcat (format, argv[3]);
  strcat (format, "s");
  int fdout = open (fname, O_RDWR | O_CREAT | O_TRUNC);
  int64_t buf_size = (N * N) * W + N;
  ftruncate (fdout, buf_size);
  char *data = mmap (NULL, buf_size + 1, PROT_WRITE | PROT_READ, MAP_SHARED,
                     fdout, 0);
  data[0] = '\0';
  int64_t *spiral_matrix = (int64_t *)calloc (N * N, sizeof (int64_t));
  FillSpiralMatrix (spiral_matrix, N, N);
  PrintMatrix (data, spiral_matrix, N, W, format, format_size);
  munmap (data, buf_size + 1);
  free (spiral_matrix);
  free (format);
  close (fdout);
  return 0;
}