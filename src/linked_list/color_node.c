#include <stdlib.h>
#include "color_node.h"

// Data structure repsonsible for colors

void free_c_node(c_node *node) {
    if (node != NULL) {
        if (node->color != NULL) {
            free_color(node->color);
            node->color = NULL;
        }
        if (node->next != NULL) {
            free_c_node(node->next);
            node->next = NULL;
        }
        free(node);
        node = NULL;
    }
}
c_node *init_c_node()
{
    c_node *node = (c_node *)malloc(sizeof(c_node));
    node->color = init_color();
    node->next = NULL;
    return node;
}