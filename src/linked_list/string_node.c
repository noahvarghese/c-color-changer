#include <stdlib.h>
#include "string_node.h"

// Yayy basic node for linked list

struct node *init_node()
{
    node *node = (struct node *)malloc(sizeof(struct node));
    node->previous = NULL;
    node->next = NULL;
    node->value = NULL;
    return node;
}