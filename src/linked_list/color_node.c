#include <stdlib.h>
#include "color_node.h"

bool rgba_is_equal(int *origin, int *compare)
{

    if (
        origin[0] == compare[0] &&
        origin[1] == compare[1] &&
        origin[2] == compare[2] &&
        origin[3] == compare[3])
    {
        return true;
    }

    return false;
}

c_node *init_c_node()
{
    c_node *node = (c_node *)malloc(sizeof(c_node));
    node->color = NULL;
    node->next = NULL;
    node->previous = NULL;
    node->occurences = 0;
    return node;
}