#include <stdint.h>
#include <stdio.h>

#define BITE_SIZE 8

int32_t
GetFirstKBits (int32_t input, int32_t k)
{
  return (input >> (BITE_SIZE - k)) & ((1 << k) - 1);
}

int
main (void)
{
  uint64_t ascii_count = 0;
  uint64_t utf_count = 0;
  int32_t expected_bites = 0;
  int32_t input_char = getchar ();
  while (input_char != EOF)
    {
      if (expected_bites == 0)
        {
          if (GetFirstKBits (input_char, 5) == 30) // 11110xxx
            {
              expected_bites = 3;
            }
          else if (GetFirstKBits (input_char, 4) == 14) // 1110xxxx
            {
              expected_bites = 2;
            }
          else if (GetFirstKBits (input_char, 3) == 6) // 110xxxxx
            {
              expected_bites = 1;
            }
          else if (GetFirstKBits (input_char, 1) == 0) // 0xxxxxxx
            {
              ++ascii_count;
            }
          else
            {
              printf ("%ld %ld", ascii_count, utf_count);
              return 1;
            }
        }
      else
        {
          if (GetFirstKBits (input_char, 2) != 2) // 10xxxxxx
            {
              printf ("%ld %ld", ascii_count, utf_count);
              return 1;
            }
          else
            {
              --expected_bites;
              if (expected_bites == 0)
                ++utf_count;
            }
        }
      input_char = getchar ();
    }
  printf ("%ld %ld", ascii_count, utf_count);
  return 0;
}