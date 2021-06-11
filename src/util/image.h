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
    int width;
    int height;
    png_structp png;
    png_infop info;
    png_bytep rows;
} image_png;

void parse_png(image_png *png);
image_png *init_png_image();
image *get_contents(char *file_name);
void png_dimensions(image *img);
void jpg_dimensions(image *img);

#endif