#include "../image/pixel.h"
#include "color_linked_list.h"

c_node *find_by_original_color(color_ll *cll, int *rgba)
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

    if (cll->length < 1)
    {
        printf("List is empty\n");
        exit(EXIT_FAILURE);
    }

    c_node *node = cll->head;

    do
    {
        if (
            rgba_is_equal(
                (png_bytep)rgba,
                node->color->original_color->type == INT ? node->color->original_color->px.i : (int *)node->color->original_color->px.p,
                10))
        {
            return node;
        }
    } while (node = node->next);

    return NULL;
}

c_node *get_most_frequent(color_ll *cll, int occurence)
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

    if (cll->length < 1)
    {
        printf("List is empty\n");
        exit(EXIT_FAILURE);
    }

    c_node *node = cll->head;
    int max = 0;
    c_node *max_node = node;

    do
    {
        if (node->occurences > max)
        {
            max = node->occurences;
            max_node = node;
        }
    } while (node = node->next);

    return node;
}

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
        if (rgba_is_equal(color, (int *)node->color->original_color->px.p, 10))
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
        if (rgba_is_equal(color, (int *)node->color->original_color->px.p, 10))
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

void append_data_to_cll(color_ll *cll, png_bytep px)
{
    color *col = init_color();
    col->original_color = (rgba *)malloc(sizeof(rgba));
    col->original_color->type = PNG_BYTEP;
    col->original_color->px.p = px;

    c_node *node = init_c_node();
    node->color = col;
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

color_ll *init_cll()
{
    color_ll *list = (color_ll *)malloc(sizeof(color_ll));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    list->size = 0;
    return list;
}
