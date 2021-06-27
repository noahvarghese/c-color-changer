#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <png.h>
#include "checks.h"
#include "../image/png.h"
#include "str.h"
#include "../linked_list/color_linked_list.h"
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
    int tolerance = 15;

    color_ll *cll = init_cll();
    image_png *png = init_png_image(file_name);
    read_png(png);
    png_stats(png, cll, tolerance);
    // modify_png(png, cll, tolerance);
    // write_png(png);
    free_png(png);
    // not implemented
    free(cll);
}