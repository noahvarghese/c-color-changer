#ifndef PATHH
#define PATHH

#include "../linked_list/string_node.h"
#include "../linked_list/string_linked_list.h"

char *path_to_dir(char *file_path);
char *path_to_charfile(char *prev_path, char *file_name);
sll *read_dir(char *dir_name);
void perform_on_files(sll *files, char prev_path[], void (*func)(char[]));

#endif