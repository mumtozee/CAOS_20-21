#include <stdint.h>

void
AlphaBlend (uint8_t under[4], uint8_t over[4], uint8_t out[4])
{
  double alpha = (double)over[0] / 255.0;
  double inv_alpha = 1.0 - alpha;
  double res_alpha = (double)under[0] * inv_alpha / 255.0 + alpha;
  out[1] = (uint8_t) (((double)under[1] * inv_alpha + (double)over[1] * alpha)
                      / res_alpha);
  out[2] = (uint8_t) (((double)under[2] * inv_alpha + (double)over[2] * alpha)
                      / res_alpha);
  out[3] = (uint8_t) (((double)under[3] * inv_alpha + (double)over[3] * alpha)
                      / res_alpha);
  out[0] = (uint8_t) (res_alpha * 255.0);
  /*out[0] = 255;
  out[1] = 0;
  out[2] = 0;
  out[3] = 255;*/
}


void
AlphaBlend2 (uint8_t under[4], uint8_t over[4], uint8_t out[4])
{
  float alpha = (float)over[0] / 255.0;
  float inv_alpha = 1.0 - alpha;
  float res_alpha = (float)under[0] * inv_alpha / 255.0 + alpha;
  out[1] = (uint8_t) (((float)under[1] * inv_alpha + (float)over[1] * alpha)
                      / res_alpha);
  out[2] = (uint8_t) (((float)under[2] * inv_alpha + (float)over[2] * alpha)
                      / res_alpha);
  out[3] = (uint8_t) (((float)under[3] * inv_alpha + (float)over[3] * alpha)
                      / res_alpha);
  out[0] = (uint8_t) (res_alpha * 255.0);
}