#include <stdio.h>
#include <stdbool.h>
#include "checks.h"
#include "image.h"

// Only deal with pngs
void color_changer(char *file_name)
{
    image *img = get_contents(file_name);

    printf("File: %s\n", file_name);
    // printf("%s\n", img->data);

    png_dimensions(img);

    printf("Width: %d\n", img->width);
    printf("Height: %d\n", img->height);

    // write to new file
}