#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "string.h"
#include <png.h>
#include "checks.h"
#include "image.h"
#include "../main.h"

// All checks for formatting are done prior
// this only supports 6 digit hex strings
int *hex_from_string(char *color)
{
    int counter = 0;
    int *return_value = (int *)malloc(sizeof(int) * 3);

    char *tmp = (char *)malloc(sizeof(char) * 3);

    for (int i = 0; i < strlen(color); i += 2)
    {
        tmp[0] = color[i];
        tmp[1] = color[i + 1];
        tmp[2] = '\0';
        return_value[counter++] = (int)strtol(tmp, NULL, 16);
    }

    return return_value;
}

// Only deal with pngs
void color_changer(char *file_name)
{
    image_png *png = init_png_image(file_name);
    parse_png(png);

    // this needs to be done after argument parsing
    // otherwise it is excess computation
    int *ignored = hex_from_string(vars->ignored_color);
    int *color = hex_from_string(vars->desired_color);

    // write to new file
}