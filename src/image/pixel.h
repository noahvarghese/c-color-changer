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
    rgba *mod_color;
    rgba *original_color;
} color;

#endif

bool color_is_equal(rgba origin_px, rgba *compare_px, int tolerance);
bool color_is_transparent(rgba px);
void rgba_to_hsv(color *px);
void hsv_to_rgba(color *px);