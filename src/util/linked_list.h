#ifndef LLH
#define LLH

typedef struct slinked_list {
    struct node *head;
    struct node *tail;
    int length;
    size_t size;
} sll;

#endif

struct slinked_list* init_sll();
struct node* get_at(struct slinked_list *ll, int index);
void append_node_to_list(struct slinked_list *list, struct node *node);
void append_data_to_list(struct slinked_list *list, char* string);
struct slinked_list* sll_from_string(char files_string[], const char files_delim);