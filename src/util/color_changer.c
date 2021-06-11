#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <png.h>
#include "checks.h"
#include "image.h"
#include "str.h"
#include "../main.h"

// Only deal with pngs
void color_changer(char *file_name)
{
    image_png *png = init_png_image(file_name);
    read_png(png);
    modify_png(png);
    write_png(png);
}