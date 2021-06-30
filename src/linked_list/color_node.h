#ifndef COLORNH
#define COLORNH

#include <stdbool.h>
#include "../image/pixel.h"

typedef struct c_node
{
    color *color;
    struct c_node *next;
} c_node;

#endif

void free_c_node(c_node *node);
c_node *init_c_node();