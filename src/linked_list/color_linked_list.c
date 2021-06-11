#include "color_linked_list.h"

bool cll_contains(color_ll *cll, int *color)
{
    c_node *node = cll->head;

    while (node != NULL)
    {
        if (rgba_is_equal(node->color, color))
        {
            return true;
        }
    }

    return false;
}

color_ll *init_cll()
{
    color_ll *list = (color_ll *)malloc(sizeof(color_ll));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    list->size = 0;
    return list;
}
