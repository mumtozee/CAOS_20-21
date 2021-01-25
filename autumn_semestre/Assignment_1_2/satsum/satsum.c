#include <stdint.h>
#include <stdio.h>

uint16_t
satsum (uint16_t x, uint16_t y)
{
  uint16_t max = (uint16_t)(~0);
  return (x > max - y) ? max : x + y;
}