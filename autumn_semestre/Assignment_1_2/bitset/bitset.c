#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#define DIGITS_SIZE 10
#define LETTERS_SIZE 26
#define TRASH_SIZE 2
#define BITSET_SIZE 62

void
SetBitValue (uint64_t *set, uint64_t idx, uint8_t value)
{
  if (value == 0)
    *set &= ~(1ULL << idx);
  else
    *set |= (1ULL << idx);
}

void
AddCharToSet (uint64_t *set, char item)
{
  if (islower (item))
    SetBitValue (set, item - 'a' + 36, 1);
  else if (isupper (item))
    SetBitValue (set, item - 'A' + 10, 1);
  else if (isdigit (item))
    SetBitValue (set, item - '0', 1);
}

uint32_t
GetBit (uint64_t set, uint64_t idx)
{
  return (set >> idx) & 1ULL;
}

void
PrintSet (uint64_t set)
{
  for (uint64_t i = 0; i < DIGITS_SIZE; ++i)
    {
      if (GetBit (set, i) == 1)
        printf ("%c", (char)('0' + i));
    }
  for (uint64_t i = 0; i < LETTERS_SIZE; ++i)
    {
      if (GetBit (set, i + DIGITS_SIZE) == 1)
        printf ("%c", (char)('A' + i));
    }
  for (uint64_t i = 0; i < LETTERS_SIZE; ++i)
    {
      if (GetBit (set, i + DIGITS_SIZE + LETTERS_SIZE) == 1)
        printf ("%c", (char)('a' + i));
    }
  printf ("\n");
}

int
main ()
{
  uint64_t bitset = 0;
  uint64_t cur_set = 0;
  char input_char = getchar ();
  while (input_char != '\n' && input_char != EOF)
    {
      switch (input_char)
        {
        case '&':
          bitset &= cur_set;
          cur_set = 0;
          break;
        case '|':
          bitset |= cur_set;
          cur_set = 0;
          break;
        case '~':
          bitset = ~bitset;
          cur_set = 0;
          break;
        case '^':
          bitset ^= cur_set;
          cur_set = 0;
          break;
        default:
          AddCharToSet (&cur_set, input_char);
          break;
        }
      input_char = getchar ();
    }
  PrintSet (bitset);
  return 0;
}