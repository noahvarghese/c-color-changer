#ifndef PIXELH
#define PIXELH

#include <png.h>
#include <stdbool.h>

typedef struct rgba
{
    int r;
    int g;
    int b;
    int a;
} rgba;

typedef struct hsv
{
    int h;
    float s;
    float v;
} hsv;

typedef struct color
{
    struct hsv *original_hsv;
    struct hsv *mod_hsv;
    struct rgba *mod_color;
    struct rgba *original_color;
} color;

#endif

color *init_color();
void copy_px_to_png_bytep(rgba *source, png_bytep dest);
void copy_png_bytep_to_rgbap(png_bytep source, rgba *dest);
// void rgba_to_hsv(color *col);
void rgba_to_hsv(rgba *rgba, hsv *hsv);
void hsv_to_rgba(hsv *hsv, rgba *rgba, int alpha);
bool compare_rgba(rgba *source, rgba *compare, int tolerance, bool include_alpha);