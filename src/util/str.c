#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "str.h"

// Any added string functionality

// All checks for formatting are done prior
// this only supports 6 digit hex strings
rgba *hex_from_string(char *color)
{
    rgba *return_value = (rgba *)malloc(sizeof(rgba) * 4);

    char *tmp = (char *)malloc(sizeof(char) * 3);

    for (int i = 0; i < strlen(color); i += 2)
    {
        tmp[0] = color[i];
        tmp[1] = color[i + 1];
        tmp[2] = '\0';
        // convert string to long using base 16
        int val = (int)strtol(tmp, NULL, 16);

        if (i == 0)
        {
            return_value->r = val;
        }
        else if (i == 2)
        {
            return_value->g = val;
        }
        else if (i == 4)
        {
            return_value->b = val;
        }
        else if (i == 6)
        {
            return_value->a = val;
        }
    }

    return return_value;
}

bool contains(char string[], const char delim)
{
    char *tmp = string;
    char *last_delim = 0;

    while (*tmp)
    {
        if (delim == *tmp)
        {
            return true;
        }
        tmp++;
    }

    return false;
}