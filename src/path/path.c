#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include "path.h"
#include "../util/checks.h"

// responsible for getting file info and performing actions on files and or directories

char *path_to_dir(char *file_path)
{
    char *dir_path;
    int dir_path_size = strlen(file_path) + 1;

    if (file_path[strlen(file_path) - 1] != '/')
        dir_path_size += 1;

    dir_path = malloc(dir_path_size);
    strcpy(dir_path, file_path);

    if (file_path[strlen(file_path) - 2] != '/')
    {
        dir_path[dir_path_size - 2] = '/';
    }

    dir_path[dir_path_size - 1] = '\0';

    return dir_path;
}

char *path_to_file(char *prev_path, char *file_name)
{
    // Leave space for null termination
    int final_path_length = strlen(prev_path) + strlen(file_name) + 1;
    char *path = (char *)malloc(final_path_length);

    char *new_path = (char *)malloc(final_path_length);

    for (int i = 0; i < strlen(prev_path); i++)
    {
        new_path[i] = prev_path[i];
    }

    int path_length = strlen(prev_path);

    for (int i = 0; i < strlen(file_name); i++)
    {
        new_path[path_length + i] = file_name[i];
    }

    new_path[final_path_length - 1] = '\0';

    return new_path;
}

sll *read_dir(char *dirname)
{
    struct dirent *dp;
    DIR *dfd;

    sll *files = init_sll();

    // Check if we can open directory
    if ((dfd = opendir(dirname)) == NULL)
    {
        printf("Can't open %s\n", dirname);
        return files;
    }

    while ((dp = readdir(dfd)) != NULL)
    {
        // If not a hidden file or the current directory
        if (dp->d_name[0] != '.')
        {

            // if not the previous directory
            if (strcmp(dp->d_name, "..") != 0)
            {
                append_data_to_slist(files, dp->d_name);
            }
        }
    }

    return files;
}
void perform_on_files(sll *files, char prev_path[], void (*func)(char[]))
{
    node *file_node = files->head;

    // Iterate over files in the directory/list
    // if it is a regular file (will have to check if it is an image type as well [png,jpg,etc])
    // If it is a directory create a new linked list and recursively call this method
    // This needs the previous path to be kept track of
    while (file_node != NULL)
    {
        char *new_path = path_to_file(prev_path, file_node->value);

        if (check_is_dir(new_path) == false)
        {
            if (check_is_file(new_path) == true)
            {
                // pngs only, jpgs are weird
                if (check_is_png(new_path) == true)
                {
                    func(new_path);
                }
            }
        }
        else
        {
            if (strlen(file_node->value) == 1 && file_node->value[0] == '.')
                continue;

            sll *ll = read_dir(new_path);
            char *dir_path = path_to_dir(new_path);
            perform_on_files(ll, dir_path, func);
        }

        file_node = file_node->next;
    }
}
