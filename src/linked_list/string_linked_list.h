#ifndef LLH
#define LLH

#include "string_node.h"
typedef struct slinked_list
{
    node *head;
    node *tail;
    int length;
    size_t size;
} sll;

#endif

sll *init_sll();
node *get_at(sll *ll, int index);
void append_node_to_slist(sll *list, node *node);
void append_data_to_slist(sll *list, char *string);
sll *sll_from_string(char files_string[], const char files_delim);