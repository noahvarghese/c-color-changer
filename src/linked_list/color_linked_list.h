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

c_node *find_by_original_color(color_ll *cll, int *rgba, int tolerance);
color_ll *init_cll();
void order_by_original_value(color_ll *cll);
// void update_color_occurence(color_ll *cll, png_bytep color, int tolerance);
bool png_bytep_exists_in_cll(color_ll *cll, png_bytep color, int tolerance);
c_node *get_at_cll(color_ll *cll, int index);
void append_data_to_cll(color_ll *cll, png_bytep color);
void append_node_to_cll(color_ll *cll, c_node *node);
// c_node *get_most_frequent(color_ll *cll, int occurence);