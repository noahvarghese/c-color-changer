#ifndef PNGH
#define PNGH

#include <png.h>

typedef struct image_png
{
    char *name;
    char *new_name;
    int width;
    int height;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *rows;
} image_png;

image_png *init_png_image();
void free_png(image_png *png);
void modify_png(image_png *png);
void write_png(image_png *png);
void read_png(image_png *png);
void png_stats(image_png *png);

#endif