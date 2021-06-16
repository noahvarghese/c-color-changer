#ifndef PIXELH
#define PIXELH

#include <png.h>
#include <stdbool.h>

union px_byte
{
    int *i;
    png_bytep p;
};

enum px_byte_type
{
    INT = 0,
    PNG_BYTEP = 1
};

typedef struct rgba
{
    union px_byte px;
    enum px_byte_type type;
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

void copy_to_px(png_bytep px, int *rgba);
color *init_color();
bool color_is_equal(rgba origin_px, rgba *compare_px, int tolerance);
void rgba_to_hsv(color *col);
void hsv_to_rgba(color *col);
bool rgba_is_equal(png_bytep image_px, int *node_px, int tolerance);