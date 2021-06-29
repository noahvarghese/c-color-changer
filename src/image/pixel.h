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
void copy_px_to_png_bytep(int *source, png_bytep dest, bool include_alpha);
bool color_is_equal(rgba origin_px, rgba *compare_px, int tolerance);
// void rgba_to_hsv(color *col);
void rgba_to_hsv(rgba *rgba, hsv* hsv);
void hsv_to_rgba(hsv *hsv, rgba* rgba, int alpha);
bool compare_rgba(rgba* source, rgba* compare, int tolerance, bool include_alpha);