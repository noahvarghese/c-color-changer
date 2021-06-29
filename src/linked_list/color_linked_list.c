#include "../image/pixel.h"
#include "color_linked_list.h"

void front_back_split(c_node *source, c_node **front_ref, c_node **back_ref)
{
    c_node *fast;
    c_node *slow;
    slow = source;
    fast = source->next;

    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front_ref = source;
    *back_ref = slow->next;
    slow->next = NULL;
}

c_node *sorted_merge(c_node *a, c_node *b)
{
    c_node *result = NULL;

    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);

    if (a->color->original_hsv->v >= b->color->original_hsv->v)
    {
        result = a;
        result->next = sorted_merge(a->next, b);
    }
    else
    {
        result = b;
        result->next = sorted_merge(a, b->next);
    }

    return (result);
}

void merge_sort(c_node **head_ref)
{
    c_node *head = *head_ref;

    if (head == NULL || head->next == NULL)
    {
        return;
    }

    c_node *a;
    c_node *b;

    front_back_split(head, &a, &b);

    merge_sort(&a);
    merge_sort(&b);
    *head_ref = sorted_merge(a, b);
}

void order_by_original_value(color_ll *cll)
{
    if (cll->head == NULL || cll->length == 1)
    {
        return;
    }

    merge_sort(&(cll->head));
}

c_node *find_by_original_color(color_ll *cll, rgba *rgba, int tolerance)
{
    if (cll->head == NULL && cll->length == 0)
    {
        return NULL;
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
            compare_rgba(
                rgba,
                node->color->original_color,
                tolerance, false))
        {
            return node;
        }
    } while (node = node->next);

    return NULL;
}

// c_node *get_most_frequent(color_ll *cll, int occurence)
// {
//     if (cll->head == NULL && cll->length == 0)
//     {
//         return NULL;
//     }

//     if (cll->length < 1)
//     {
//         printf("List is empty\n");
//         exit(EXIT_FAILURE);
//     }

//     c_node *node = cll->head;
//     int max = 0;
//     c_node *max_node = node;

//     do
//     {
//         if (node->occurences > max)
//         {
//             max = node->occurences;
//             max_node = node;
//         }
//     } while (node = node->next);

//     return node;
// }

// void update_color_occurence(color_ll *cll, png_bytep color, int tolerance)
// {
//     if (cll->head == NULL && cll->length == 0)
//     {
//         return;
//     }

//     if (cll->length < 1)
//     {
//         printf("List is empty\n");
//         exit(EXIT_FAILURE);
//     }

//     c_node *node = cll->head;

//     do
//     {
//         int *original_color = (int *)malloc(sizeof(int) * 4);
//         original_color[0] = node->color->original_color->px.p[0];
//         original_color[1] = node->color->original_color->px.p[1];
//         original_color[2] = node->color->original_color->px.p[2];

//         if (rgba_is_equal(color, original_color, tolerance))
//         {
//             node->occurences++;
//             return;
//         }
//     } while (node = node->next);

//     return;
// }

bool png_bytep_exists_in_cll(color_ll *cll, png_bytep color, int tolerance)
{
    if (cll->head == NULL && cll->length == 0)
    {
        return false;
    }

    c_node *node = cll->head;

    do
    {
        rgba *source = (rgba *)malloc(sizeof(rgba));
        copy_png_bytep_to_rgbap(color, source);

        if (compare_rgba(source, node->color->original_color, tolerance, false))
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
    col->original_hsv = (hsv *)malloc(sizeof(hsv));
    copy_png_bytep_to_rgbap(px, col->original_color);
    rgba_to_hsv(col->original_color, col->original_hsv);

    c_node *node = init_c_node();
    node->color = col;
    append_node_to_cll(cll, node);
}

void push_node(color_ll *cll, c_node *node)
{
    node->next = cll->head;
    cll->head = node->next;
    cll->length++;
}

void push_data(color_ll *cll, png_bytep px)
{
    color *col = init_color();
    col->original_color = (rgba *)malloc(sizeof(rgba));
    copy_png_bytep_to_rgbap(px, col->original_color);
    rgba_to_hsv(col->original_color, col->original_hsv);

    c_node *node = init_c_node();
    node->color = col;

    push_node(cll, node);
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
