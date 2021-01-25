void
BitMapImage::Blend (const BitMapImage &foreground, unsigned int x,
                    unsigned int y)
{
  unsigned char *bkg_ptr = image.get ();
  unsigned char *frg_ptr = foreground.image.get ();

  const __m128i low_pixel_line_half = _mm_setr_epi8 (
      0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7, 0x80);
  const __m128i high_pixel_line_half
      = _mm_setr_epi8 (8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80, 12, 0x80, 13,
                       0x80, 14, 0x80, 15, 0x80);
  const __m128i alpha_mask
      = _mm_setr_epi8 (6, 0x80, 6, 0x80, 6, 0x80, 6, 0x80, 14, 0x80, 14, 0x80,
                       14, 0x80, 14, 0x80);
  const __m128i store_low_half
      = _mm_setr_epi8 (1, 3, 5, 0x80, 9, 11, 13, 0x80, 0x80, 0x80, 0x80, 0x80,
                       0x80, 0x80, 0x80, 0x80);
  const __m128i store_high_half
      = _mm_setr_epi8 (0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 1, 3, 5,
                       0x80, 9, 11, 13, 0x80);

  for (unsigned int ycur = 0; ycur < foreground.height; ycur++)
    {
      for (unsigned int xcur = 0; xcur < foreground.width - 8; xcur += 8)
        {

          unsigned int bkg_pos = ((y + ycur) * width + x + xcur) << 2;
          unsigned int frg_pos = (ycur * foreground.width + xcur) << 2;

          /*
           *
           * Background: |A3|R3|G3|B3|
           *             |A2|R2|G2|B2|
                         |A1|R1|G1|B1|
                         |A0|R0|G0|B0|

             Foreground: |A3|R3|G3|B3|
           *             |A2|R2|G2|B2|
                         |A1|R1|G1|B1|
                         |A0|R0|G0|B0|
           */
          __m128i bkg = _mm_load_si128 ((__m128i *)(bkg_ptr + bkg_pos));
          __m128i frg = _mm_load_si128 ((__m128i *)(frg_ptr + frg_pos));

          __m128i bkg1 = _mm_shuffle_epi8 (bkg, low_pixel_line_half);
          __m128i bkg2 = _mm_shuffle_epi8 (bkg, high_pixel_line_half);

          __m128i frg1 = _mm_shuffle_epi8 (frg, low_pixel_line_half);
          __m128i frg2 = _mm_shuffle_epi8 (frg, high_pixel_line_half);

          /*
           * Diff 1: |__A1|__R1| |__G1|__B1| |__A0|__R0|
           |__G0|__B0|
           * Diff 2: |__A3|__R3| |__G3|__B3| |__A2|__R2|
           |__G2|__B2|
           */

          __m128i diff1 = _mm_sub_epi16 (frg1, bkg1);
          __m128i diff2 = _mm_sub_epi16 (frg2, bkg2);

          /*
           * Prepare alphas
           * Alpha 1: |__A1|__A1| |__A1|__A1| |__A0|__A0|
           |__A0|__A0|
           * Alpha 2: |__A3|__A3| |__A3|__A3| |__A2|__A2|
           |__A2|__A2|
           */
          __m128i alpha1 = _mm_shuffle_epi8 (frg1, alpha_mask);
          __m128i alpha2 = _mm_shuffle_epi8 (frg2, alpha_mask);
          // multiply alphas
          diff1 = _mm_mullo_epi16 (diff1, alpha1);
          diff2 = _mm_mullo_epi16 (diff2, alpha2);
          // extract result bytes from diffs
          __m128i res1 = _mm_shuffle_epi8 (diff1, store_low_half);
          __m128i res2 = _mm_shuffle_epi8 (diff2, store_high_half);
          __m128i result = _mm_add_epi8 (res1, res2);
          result = _mm_add_epi8 (result, bkg);

          _mm_store_si128 ((__m128i *)(bkg_ptr + bkg_pos), result);
        }
    }
}

static const __m128i get_alpha_low = _mm_set_epi8 (
    0xFF, 7, 0xFF, 7, 0xFF, 7, 0xFF, 7, 0xFF, 3, 0xFF, 3, 0xFF, 3, 0xFF, 3);

static const __m128i get_alpha_high
    = _mm_set_epi8 (0xFF, 15, 0xFF, 15, 0xFF, 15, 0xFF, 15, 0xFF, 11, 0xFF, 11,
                    0xFF, 11, 0xFF, 11);

static const __m128i ones
    = _mm_set_epi8 (0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

for (int y = 0; y < top.height; ++y)
  {
    for (int x = 0; x < top.width; x += 4)
      {
        __m128i front_pixel = _mm_loadu_si128 (
            reinterpret_cast<const __m128i *> (top.pixels + pixel_size * x));
        __m128i back_pixel = _mm_loadu_si128 (
            reinterpret_cast<const __m128i *> (base.pixels + pixel_size * x));

        __m128i back_top_two = _mm_movehl_ps (
            back_pixel, back_pixel); // неявное приведение типов

        __m128i back_bot_two_16 = _mm_cvtepu8_epi16 (back_pixel);
        __m128i back_top_two_16 = _mm_cvtepu8_epi16 (back_top_two);

        __m128i front_alpha_low
            = _mm_shuffle_epi8 (front_pixel, get_alpha_low);
        __m128i front_alpha_high
            = _mm_shuffle_epi8 (front_pixel, get_alpha_high);

        __m128i back_coefs_low = _mm_sub_epi8 (ones, front_alpha_low);
        __m128i back_coefs_high = _mm_sub_epi8 (ones, front_alpha_high);

        __m128i back_new_low
            = _mm_mulhi_epu16 (back_bot_two_16, back_coefs_low);
        __m128i back_new_high
            = _mm_mulhi_epu16 (back_top_two_16, back_coefs_high);

        __m128i back_new = _mm_packus_epi16 (back_new_low, back_new_high);

        back_new = _mm_adds_epi8 (front_pixel, back_new);

        _mm_storeu_si128 ((__m128i *)(base.pixels + pixel_size * x), back_new);
      }
    top.pixels += pixel_size * top.width;
    base.pixels += pixel_size * base.width;
  }

FILE *result = fopen (result_image, "wb");
fwrite (base_data, sizeof (char), count_symbols (base_image), result);
fclose (result);

free ((void *)base_data);
free ((void *)top_data);
}

const static __m128i zero
    = _mm_set_epi8 (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
const static __m128i alpha = _mm_set_epi8 (255, 14, 255, 14, 255, 14, 255, 14,
                                           255, 6, 255, 6, 255, 6, 255, 6);
const static __m128i extend_255 = _mm_set_epi8 (
    0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255);

__m128i front_pxls = _mm_lddqu_si128 (front); // 4 pixels in 16 bytes
__m128i back_pxls = _mm_lddqu_si128 (back);

__m128i low_pixels_b = reinterpret_cast<__m128i> (_mm_movelh_ps (
    reinterpret_cast<__m128> (back_pxls),
    reinterpret_cast<__m128> (
        zero))); // zero in upper 8 bytes and 2 pixels in lower 8 bytes

__m128i low_pixels_f = reinterpret_cast<__m128i> (_mm_movelh_ps (
    reinterpret_cast<__m128> (front_pxls), reinterpret_cast<__m128> (zero)));

// extend low pixels for correct multiplication
low_pixels_b = _mm_cvtepu8_epi16 (low_pixels_b);
low_pixels_f = _mm_cvtepu8_epi16 (low_pixels_f);

__m128i low_alpha_f = _mm_shuffle_epi8 (low_pixels_f, alpha);

low_pixels_b
    = _mm_mullo_epi16 (low_pixels_b, _mm_sub_epi16 (extend_255, low_alpha_f));
low_pixels_b = _mm_add_epi16 (low_pixels_b,
                              _mm_mullo_epi16 (low_pixels_f, low_alpha_f));
low_pixels_b = _mm_srli_epi16 (low_pixels_b, 8);

low_pixels_b = _mm_shuffle_epi8 (
    low_pixels_b, _mm_set_epi8 (255, 255, 255, 255, 255, 255, 255, 255, 14, 12,
                                10, 8, 6, 4, 2, 0));

__m128i up_pixels_b = reinterpret_cast<__m128i> (_mm_movehl_ps (
    reinterpret_cast<__m128> (zero), reinterpret_cast<__m128> (back_pxls)));

__m128i up_pixels_f = reinterpret_cast<__m128i> (_mm_movehl_ps (
    reinterpret_cast<__m128> (zero), reinterpret_cast<__m128> (front_pxls)));

// extend low pixels for correct multiplication
up_pixels_b = _mm_cvtepu8_epi16 (low_pixels_b);
up_pixels_f = _mm_cvtepu8_epi16 (low_pixels_f);

__m128i up_alpha_f = _mm_shuffle_epi8 (up_pixels_f, alpha);

up_pixels_b
    = _mm_mullo_epi16 (up_pixels_b, _mm_sub_epi16 (extend_255, up_alpha_f));
up_pixels_b
    = _mm_add_epi16 (up_pixels_b, _mm_mullo_epi16 (up_pixels_f, up_alpha_f));
up_pixels_b = _mm_srli_epi16 (up_pixels_b, 8);

up_pixels_b = _mm_shuffle_epi8 (
    up_pixels_b, _mm_set_epi8 (14, 12, 10, 8, 6, 4, 2, 0, 255, 255, 255, 255,
                               255, 255, 255, 255));

back_pxls = _mm_or_si128 (up_pixels_b, low_pixels_b);

_mm_storeu_si128 (back, back_pxls);

unsigned int alpha = fg[3] + 1;
unsigned int inv_alpha = 256 - fg[3];
result[0] = (unsigned char)((alpha * fg[0] + inv_alpha * bg[0]) >> 8);
result[1] = (unsigned char)((alpha * fg[1] + inv_alpha * bg[1]) >> 8);
result[2] = (unsigned char)((alpha * fg[2] + inv_alpha * bg[2]) >> 8);
result[3] = 0xff;