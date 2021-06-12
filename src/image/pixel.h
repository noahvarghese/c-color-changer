#ifndef PIXELH
#define PIXELH

#include <png.h>
#include <stdbool.h>

#endif

bool png_bytep_is_equal(png_bytep origin_px, int *compare_px, int tolerance);
bool intp_is_equal(int *origin, int *compare);
void copy_to_png_bytep(png_bytep dest, int *src);
void copy_to_intp(int *dest, png_bytep src);
bool png_bytep_is_transparent(png_bytep px);
bool intp_is_transparent(int *px);
png_bytep png_bytep_from_intp(int *px);
int *intp_from_png_bytep(png_bytep px);