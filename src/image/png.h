#ifndef PNGH
#define PNGH

#include <png.h>
#include "../linked_list/color_linked_list.h"

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
void calc_output_colors(color_ll *cll);
void modify_png(image_png *png, color_ll *cll, int tolerance);
void write_png(image_png *png);
void read_png(image_png *png);
void png_stats(image_png *png, color_ll *cll, int tolerance);

#endif