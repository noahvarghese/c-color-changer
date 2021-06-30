#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../util/str.h"
#include "string_linked_list.h"
#include "../main.h"

// Utilized while parsing CLI args (specifically for checking paths are correct), couldve maybe done a tree?

sll *init_sll()
{
    sll *list = (sll *)malloc(sizeof(sll));

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    list->size = 0;

    return list;
}

node *get_at_sll(sll *ll, int index)
{
    if (ll->head == NULL && ll->length == 0)
    {
        return false;
    }

    if (ll->head->previous != NULL)
    {
        printf("Node is not the head\n");
        exit(EXIT_FAILURE);
    }

    if (index < 0)
    {
        printf("Index cannot be less than 0\n");
        exit(EXIT_FAILURE);
    }

    if (index > ll->length - 1)
    {
        printf("Index is greater than the length.");
        exit(EXIT_FAILURE);
    }

    if (index == 0)
    {
        return ll->head;
    }

    if (index == ll->length - 1)
    {
        return ll->tail;
    }

    int count = 0;
    node *node = ll->head;

    do
    {
        if (count == index)
        {
            return node;
        }
        node = node->next;
        count++;
    } while (node->next != NULL);

    return NULL;
}

void append_node_to_slist(sll *list, struct node *node)
{
    // handles empty list
    if (list->head == NULL)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;
        node->previous = list->tail;
        list->tail = node;
    }

    list->length++;
}

void append_data_to_slist(sll *list, char *data)
{
    node *node = init_node();
    node->value = strdup(data);
    append_node_to_slist(list, node);
}

sll *sll_from_string(char files[], const char _delim)
{
    sll *paths = init_sll();

    char delim[] = {_delim, '\0'};

    char *rest = strdup(files);
    char *token;

    paths->head = init_node();
    struct node *current = paths->head;

    if (contains(files, _delim) == false)
    {
        paths->head->value = strdup(files);
        return paths;
    }

    while (token = strtok_r(files, delim, &rest))
    {
        printf("Current file: %s\n", token);
        current->value = token;
        current->next = init_node();

        struct node *tmp = current->next;
        tmp->previous = current;
        current = tmp;

        paths->length++;
    }

    return paths;
}