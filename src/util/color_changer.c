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

// handles program flow

void calc_output_colors(color_ll *cll)
{
    order_by_original_value(cll);

    if (cll->head == NULL)
    {
        return;
    }

    cll->head->color->mod_color = vars->mod_rgba;
    cll->head->color->mod_hsv = vars->mod_hsv;

    c_node *prev = cll->head;
    c_node *next = prev->next;

    while (next != NULL)
    {

        next->color->mod_hsv->h = prev->color->mod_hsv->h;
        next->color->mod_hsv->s = prev->color->mod_hsv->s;
        next->color->mod_hsv->v = prev->color->mod_hsv->v - (prev->color->original_hsv->v - next->color->original_hsv->v);
        hsv_to_rgba(next->color->mod_hsv, next->color->mod_color, -1);

        prev = next;
        next = prev->next;
    }
}

void color_changer(char *file_name)
{
    color_ll *cll = init_cll();
    image_png *png = init_png_image(file_name);
    read_png(png);
    png_stats(png, cll, vars->tolerance);
    calc_output_colors(cll);
    modify_png(png, cll, vars->tolerance);
    write_png(png);
    free_png(png);
    // Error gets thrown don't know why
    // free_cll(cll);
}