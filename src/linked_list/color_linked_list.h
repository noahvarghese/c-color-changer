#ifndef COLORH
#define COLORH

#include <stdbool.h>
#include <stdlib.h>
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
bool cll_contains(color_ll *cll, int *color);