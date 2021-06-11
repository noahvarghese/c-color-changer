#ifndef IMAGEH
#define IMAGEH

#include <stdio.h>
#include <png.h>

typedef struct image
{
    char *file_name;
    char *data;
    int data_length;
    unsigned int width;
    unsigned int height;
} image;

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

void modify_png(image_png *png);
void write_png(image_png *png);
void read_png(image_png *png);
image_png *init_png_image();
image *get_contents(char *file_name);
void png_dimensions(image *img);
void jpg_dimensions(image *img);

#endif