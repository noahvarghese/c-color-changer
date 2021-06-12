#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <png.h>
#include "checks.h"
#include "../image/png.h"
#include "str.h"
#include "../main.h"

void calc_hues(image_png *png)
{
    // based off # of colors as well as the range
    // calculate x darker colors based of desired_color
    // where x is # of colors - 1 (accounts for the color given by the user)
}

// Only deal with pngs
void color_changer(char *file_name)
{
    image_png *png = init_png_image(file_name);
    read_png(png);
    png_stats(png);
    modify_png(png);
    write_png(png);
    free_png(png);
}