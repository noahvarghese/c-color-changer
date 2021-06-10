#ifndef LLH
#define LLH

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
void append_node_to_list(sll *list, node *node);
void append_data_to_list(sll *list, char *string);
sll *sll_from_string(char files_string[], const char files_delim);