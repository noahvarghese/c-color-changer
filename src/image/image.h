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

char *modify_image_name(char *file_name);
image *get_contents(char *file_name);
void png_dimensions(image *img);
void jpg_dimensions(image *img);

#endif