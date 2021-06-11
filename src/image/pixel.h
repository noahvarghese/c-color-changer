#ifndef PIXELH
#define PIXELH

#include <png.h>
#include <stdbool.h>

#endif

bool png_byte_is_equal(png_bytep origin_px, int *compare_px);
void copy_pixel(png_bytep dest, int *src);
bool is_transparent(png_bytep px);