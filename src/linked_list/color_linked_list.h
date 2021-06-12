#ifndef COLORH
#define COLORH

#include <stdbool.h>
#include <stdlib.h>
#include <png.h>
#include "color_node.h"

typedef struct color_linked_list
{
    c_node *head;
    c_node *tail;
    int length;
    size_t size;
} color_ll;

#endif

color_ll *init_cll();
void update_color_occurence(color_ll *cll, png_bytep color);
bool png_bytep_exists_in_cll(color_ll *cll, png_bytep color);
bool intp_exists_in_cll(color_ll *cll, int *color);
c_node *get_at_cll(color_ll *cll, int index);
bool cll_contains(color_ll *cll, int *color);
void append_png_bytep_to_cll(color_ll *cll, png_bytep color);
void append_intp_to_cll(color_ll *cll, int *color);
void append_node_to_cll(color_ll *cll, c_node *node);