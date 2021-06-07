#ifndef GLOBAL_H 
#define GLOBAL_H

typedef struct program_vars {
    char *ignored_color;
    char *desired_color;
    char *path;
    bool is_dir;
} program_vars;

extern program_vars *vars;
#endif