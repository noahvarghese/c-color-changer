#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "./util/args.h"
#include "./util/checks.h"
#include "./util/color_changer.h"
#include "./util/node.h"
#include "./util/linked_list.h"
#include "main.h"

program_vars *vars;

sll* read_dir(char* dirname) {
    struct dirent *dp;
    DIR *dfd;

    sll *files = init_sll();

    // Check if we can open directory
    if ((dfd = opendir(dirname)) == NULL) {
        printf("Can't open %s\n", dirname);
        return files;
    }

    while((dp = readdir(dfd)) != NULL) {
        // If not a hidden file or the current directory
        if ( dp->d_name[0] != '.' ) {

            // if not the previous directory
            if (strcmp(dp->d_name, "..") != 0) {
                append_data_to_list(files, dp->d_name);
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
    while ( file_node != NULL ) {
        printf("File: %s\n", file_node->value);

        if ( check_is_dir(file_node->value) == false ) {
            if ( check_is_file(file_node->value) == true ) 
                func(file_node->value);

            return;
        }

        if ( strlen(file_node->value) == 1 && file_node->value[0] == '.' )
            return;

        if (prev_path != "") {
            if (prev_path[(int)strlen(prev_path) - 1] != '/') {
                strcat(prev_path, "/");
            }
            strcat(prev_path, file_node->value);
        }
        else {
            bool concatenated = false;

            if (file_node->value[0] != '.' && file_node->value[1] != '/') {
                if (file_node->value[0] != '/') {
                    prev_path = "./";
                    strcat(prev_path, file_node->value);
                    concatenated = true;
                }
            }
            else {
                if (concatenated == false) {
                    prev_path = strdup(file_node->value);
                }
            }
        } 
        
        printf("prev_path: %s\n", prev_path);

        // get struct of files
        // pass to perform on files
        char *full_file_path = malloc((strlen(prev_path) + strlen(file_node->value)) * sizeof(char) + 1);
        sprintf(full_file_path, "%s%s", prev_path, file_node->value);
        printf("File path: %s\n", full_file_path);

        sll *ll = read_dir(file_node->value);
        printf("File count: %d\n", ll->length);
        perform_on_files(ll, prev_path, func);

        file_node = file_node->next;
    }
}

int close_program(bool success)
{
    printf("Success: %s\n", success == 1 ? "true" : "false");
    free(vars);
    return success;
}

void tmp_func(char yolo[]) {}

int main(int argc, char *argv[])
{
    vars = (program_vars*) malloc(sizeof(program_vars));
    bool success = false;
    // Parse cli args
    // -i/--ignore ignored color
    // -c/--color desired color
    // -p/--path image path (directory or file)

    if (parse_args(argc, argv) == false)
    {
        printf(HELP_MESSAGE);
        return close_program(success);
    }

    const char delim = ',';
    sll *files = sll_from_string(vars->path, delim);

    perform_on_files(files, "", tmp_func);

    // For each file

    // Loop through pixels
    // if pixel != transparent AND pixel != ignored color
    // Get 'largest' and 'smallest' color value
    // (largest = white)
    // (smallest = black)
    // Calculate 4 ranges starting at 'largest'

    // Create new file
    // Loop through pixels again
    // if pixel == transparent AND pixel == ignored color
    // Save pixel to new file
    // else
    // check which of the 4 ranges the pixel falls in
    // save calculated color pixel to file

    // switch order as user defined functions 1 = true, and 0 = false
    // but in main 0 means no errors, while 1 means error occurred 
    return close_program(true) == 1 ? 0 : 1;
}
