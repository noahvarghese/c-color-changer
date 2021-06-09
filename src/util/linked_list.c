#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "node.h"
#include "linked_list.h"
#include "../main.h"

sll* init_sll() {
    sll *list;

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    list->size = 0;

    return list;
}

node* get_at(sll *ll, int index) {

    if ( ll->head->previous != NULL ) {
        printf("Node is not the head\n");
        exit(8);
    }

    if ( index == 0 ) {
        return ll->head;
    }

    int count = 0;
    node *node = ll->head;

    while (node->next != NULL) {
        if ( count == index ) {
            return node;
        }
        node = node->next;
        count++;
    }

    return NULL;
}

void append_node_to_list(sll *list, struct node *node) {
    // handles empty list
    if ( list->head == NULL ) {
        list->head = node;
        list->tail = node;
    }
    else {
        list->tail->next = node;
        node->previous = list->tail;
        list->tail = node;
    }
    
    list->length++;
}

void append_data_to_list(sll *list, char *data) {
    node *node = init_node();
    node->value = strdup(data);
    append_node_to_list(list, node);
}

sll* sll_from_string(char files[], const char _delim)
{
    sll *paths = init_sll();

    char delim[] = {_delim, '\0'};

    char *rest = strdup(files);
    char *token;

    paths->head = init_node();
    struct node *current = paths->head;

    while ( token = strtok_r(files, delim, &rest) )  {
        current->value = token;
        current->next = init_node();

        struct node *tmp = current->next;
        tmp->previous = current;
        current = tmp;

        paths->length++;
    }

    return paths;
}