#include <stdlib.h>
#include "color_node.h"

c_node *init_c_node()
{
    c_node *node = (c_node *)malloc(sizeof(c_node));
    node->color = NULL;
    node->next = NULL;
    return node;
}