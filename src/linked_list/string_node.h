#ifndef NODEH
#define NODEH

// Need to make sure there is always a null pointer at root's previous
// Need to make sure there is always a null pointer as next

typedef struct node
{
    // Make sure to null terminate and leave space (strlen + 1)
    // Make it the full path
    char *value;
    struct node *previous;
    struct node *next;
} node;

#endif

node *init_node();
