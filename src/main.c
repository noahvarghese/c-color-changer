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
#include "./linked_list/string_node.h"
#include "./linked_list/string_linked_list.h"
#include "./path/path.h"
#include "main.h"

program_vars *vars;

int close_program(bool success)
{
    printf("\nSuccess: %s\n\n", success == 1 ? "true" : "false");
    free(vars);
    return success == 1 ? EXIT_SUCCESS : EXIT_FAILURE;
}

void free_vars() {
    if (vars != NULL) {
        if (vars->ignored_hex != NULL){
            free(vars->ignored_hex);    
            vars->ignored_hex = NULL;
        }

        if (vars->ignored_rgba != NULL) {
            free(vars->ignored_rgba);
            vars->ignored_rgba = NULL;
        }

        if (vars->mod_hex != NULL) {
            free(vars->mod_hex);
            vars->mod_hex = NULL;
        }

        if (vars->mod_hsv != NULL ) {
            free(vars->mod_hsv);
            vars->mod_hsv = NULL;
        }

        if (vars->mod_rgba != NULL) {
            free(vars->mod_rgba);
            vars->mod_rgba = NULL;
        }

        if (vars->output_path != NULL) {
            free(vars->output_path);
            vars->output_path = NULL;
        }

        if (vars->path != NULL) {
            free(vars->path);
            vars->path = NULL;
        }
        free(vars);
    }
}

int main(int argc, char *argv[])
{
    vars = (program_vars *)malloc(sizeof(program_vars));
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

    perform_on_files(files, "", color_changer);

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

    // Error gets thrown don't know why
    // free_vars();
    return close_program(true) == 1 ? 0 : 1;
}
