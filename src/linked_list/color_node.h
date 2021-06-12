#ifndef COLORNH
#define COLORNH

#include <stdbool.h>

typedef struct c_node
{
    int *color;
    int occurences;
    struct c_node *previous;
    struct c_node *next;
} c_node;

#endif

bool rgba_is_equal(int *origin, int *compare);
c_node *init_c_node();