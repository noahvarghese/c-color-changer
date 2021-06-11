#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "str.h"

// All checks for formatting are done prior
// this only supports 6 digit hex strings
int *hex_from_string(char *color)
{
    int counter = 0;
    int *return_value = (int *)malloc(sizeof(int) * 4);

    char *tmp = (char *)malloc(sizeof(char) * 3);

    for (int i = 0; i < strlen(color); i += 2)
    {
        tmp[0] = color[i];
        tmp[1] = color[i + 1];
        tmp[2] = '\0';
        // convert string to long using base 16
        return_value[counter++] = (int)strtol(tmp, NULL, 16);
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