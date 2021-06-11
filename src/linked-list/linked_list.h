#ifndef LLH
#define LLH

#include "node.h"

typedef struct color_linked_list
{
    c_node *head;
    c_node *tail;
    int length;
    size_t size;
} color_ll;

typedef struct slinked_list
{
    node *head;
    node *tail;
    int length;
    size_t size;
} sll;

#endif

color_ll *init_cll();
sll *init_sll();
node *get_at(sll *ll, int index);
void append_node_to_list(sll *list, node *node);
void append_data_to_list(sll *list, char *string);
sll *sll_from_string(char files_string[], const char files_delim);