#include "../image/pixel.h"
#include "color_linked_list.h"

void update_color_occurence(color_ll *cll, png_bytep color)
{
    if (cll->head == NULL && cll->length == 0)
    {
        return;
    }

    if (cll->head->previous != NULL)
    {
        printf("Node is not the head\n");
        exit(EXIT_FAILURE);
    }

    if (cll->length < 1)
    {
        printf("List is empty\n");
        exit(EXIT_FAILURE);
    }

    c_node *node = cll->head;

    do
    {
        if (png_bytep_is_equal(color, node->color, 10))
        {
            node->occurences++;
            return;
        }
    } while (node = node->next);

    return;
}

bool png_bytep_exists_in_cll(color_ll *cll, png_bytep color)
{
    if (cll->head == NULL && cll->length == 0)
    {
        return false;
    }

    if (cll->head->previous != NULL)
    {
        printf("Node is not the head\n");
        exit(EXIT_FAILURE);
    }

    c_node *node = cll->head;

    do
    {
        if (png_bytep_is_equal(color, node->color, 10))
        {
            return true;
        }
    } while (node = node->next);

    return false;
}

bool intp_exists_in_cll(color_ll *cll, int *color)
{
    if (cll->head == NULL && cll->length == 0)
    {
        return false;
    }

    if (cll->head->previous != NULL)
    {
        printf("Node is not the head\n");
        exit(EXIT_FAILURE);
    }

    c_node *node = cll->head;

    do
    {
        if (intp_is_equal(color, node->color))
        {
            return true;
        }
    } while (node = node->next);

    return false;
}

c_node *get_at_cll(color_ll *cll, int index)
{
    if (cll->head == NULL && cll->length == 0)
    {
        return NULL;
    }

    if (cll->head->previous != NULL)
    {
        printf("Node is not the head\n");
        exit(EXIT_FAILURE);
    }

    if (index < 0)
    {
        printf("Index cannot be less than 0\n");
        exit(EXIT_FAILURE);
    }

    if (index > cll->length - 1)
    {
        printf("Index is greater than the length.");
        exit(EXIT_FAILURE);
    }

    if (index == 0)
    {
        return cll->head;
    }

    if (index == cll->length - 1)
    {
        return cll->tail;
    }

    int count = 0;
    c_node *node = cll->head;

    do
    {
        if (count == index)
        {
            return node;
        }
        count++;
    } while (node = node->next);

    return NULL;
}

void append_png_bytep_to_cll(color_ll *cll, png_bytep color)
{
    int *data = intp_from_png_bytep(color);

    c_node *node = init_c_node();
    node->color = data;
    node->occurences = 1;
    append_node_to_cll(cll, node);
}

void append_intp_to_cll(color_ll *cll, int *color)
{
    c_node *node = init_c_node();
    node->color = color;
    node->occurences = 1;
    append_node_to_cll(cll, node);
}

void append_node_to_cll(color_ll *cll, c_node *node)
{
    if (cll->head == NULL)
    {
        cll->head = node;
        cll->tail = node;
    }
    else
    {
        cll->tail->next = node;
        node->previous = cll->tail;
        cll->tail = node;
    }

    cll->length++;
}

bool cll_contains(color_ll *cll, int *color)
{
    c_node *node = cll->head;

    while (node != NULL)
    {
        if (rgba_is_equal(node->color, color))
        {
            return true;
        }
    }

    return false;
}

color_ll *init_cll()
{
    color_ll *list = (color_ll *)malloc(sizeof(color_ll));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    list->size = 0;
    return list;
}
