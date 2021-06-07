#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "checks.h"
#include "../main.h"

#ifndef HELP_MESSAGE
#define HELP_MESSAGE "-i/--ignore\tis the color to be ignored in hex notation\n-c/--color\tthe color to replace with\n-p/--path\tthe path to the file(s) to be changed\n" 
#endif

#ifndef IGNORED_MESSAGE
#define IGNORED_MESSAGE "Ignored color not in correct format\n"
#endif

#ifndef COLOR_MESSAGE
#define COLOR_MESSAGE "Desired color not in correct format\n"
#endif

#ifndef PATH_NEXISTS_MESSAGE
#define PATH_NEXISTS_MESSAGE "File path does not exist\n"
#endif

#ifndef PATH_NMATCH_MESSAGE
#define PATH_NMATCH_MESSAGE "File is not regular file or directory\n"
#endif

bool parse_args(int argc, char *argv[]);