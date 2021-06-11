#include <stdlib.h>
#include "string_node.h"

struct node *init_node()
{
    node *node = (struct node *)malloc(sizeof(struct node));
    node->previous = NULL;
    node->next = NULL;
    node->value = NULL;
    return node;
}