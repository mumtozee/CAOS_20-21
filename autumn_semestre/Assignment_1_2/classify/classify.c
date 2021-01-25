#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MANTISSA_SIZE 52
#define EXP_SIZE 11

typedef enum
{
  PlusZero = 0x00,
  MinusZero = 0x01,
  PlusInf = 0xF0,
  MinusInf = 0xF1,
  PlusRegular = 0x10,
  MinusRegular = 0x11,
  PlusDenormal = 0x20,
  MinusDenormal = 0x21,
  SignalingNaN = 0x30,
  QuietNaN = 0x31
} float_class_t;

typedef union
{
  double double_value;
  struct
  {
    uint64_t mantissa : MANTISSA_SIZE;
    uint64_t exp : EXP_SIZE;
    uint64_t sign : 1;
  };
} double_parser_t;

extern float_class_t
classify (double *value_ptr)
{
  double value_to_process = *value_ptr;
  double_parser_t parser = { .double_value = value_to_process };
  int8_t sign = (int8_t)parser.sign;
  uint16_t exp = (uint16_t)parser.exp;
  uint64_t mantissa = (uint64_t)parser.mantissa;
  if (exp == (1ULL << EXP_SIZE) - 1ULL)
    {
      if (mantissa == 0)
        return (sign == 1) ? MinusInf : PlusInf;

      return ((mantissa >> (MANTISSA_SIZE - 1)) == 0) ? SignalingNaN
                                                      : QuietNaN;
    }
  else if (exp == 0)
    {
      if (mantissa == 0)
        return (sign == 1) ? MinusZero : PlusZero;

      return (sign == 1) ? MinusDenormal : PlusDenormal;
    }
  return (sign == 1) ? MinusRegular : PlusRegular;
}

void
foo (float_class_t var)
{
  switch (var)
    {
    case PlusZero:
      printf ("+0");
      break;
    case MinusZero:
      printf ("-0");
      break;
    case PlusInf:
      printf ("+inf");
      break;
    case MinusInf:
      printf ("-inf");
      break;
    case PlusRegular:
      printf ("+reg");
      break;
    case MinusRegular:
      printf ("-reg");
      break;
    case PlusDenormal:
      printf ("+Denormal");
      break;
    case MinusDenormal:
      printf ("-Denormal");
      break;
    case SignalingNaN:
      printf ("SignalingNan");
      break;
    case QuietNaN:
      printf ("QuietNan");
      break;
    }
  printf ("\n");
}

int
main ()
{
  // double x = 4.94066e-324;
  double x = 1;
  foo (classify (&x));
  return 0;
}