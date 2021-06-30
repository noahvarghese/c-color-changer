#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdbool.h>
#include "./image/pixel.h"

typedef struct program_vars
{
    hsv *mod_hsv;
    rgba *ignored_rgba;
    rgba *mod_rgba;
    // hex representation
    // user entered string
    char *ignored_hex;
    char *mod_hex;
    char *path;
    bool is_dir;
    char *output_path;
    int tolerance;
} program_vars;

extern program_vars *vars;
#endif