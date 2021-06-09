#include <stdbool.h>
#include "str.h"

bool contains(char string[], const char delim) {
    char *tmp = string;
    char *last_delim = 0;

    while (*tmp) {
        if (delim == *tmp) {
            return true;
        }
        tmp++;
    }

    return false;
}