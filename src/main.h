#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdbool.h>

typedef struct program_vars
{
    int *ignored;
    int *desired;
    char *ignored_color;
    char *desired_color;
    char *path;
    bool is_dir;
} program_vars;

extern program_vars *vars;
#endif