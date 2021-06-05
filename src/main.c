#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <dirent.h>

#include "./util/args.h"
#include "./util/vars.h"
#include "./util/checks.h"
#include "./util/color_changer.h"

char **get_files(char dir[])
{
    char *files[];

    return files;
}

void perform_on_files(int files_count, char *files[], char prev_path[], void (*func)(char[]))
{
    int size = files_count / sizeof files[0];

    for (int i = 0; i < size; i++)
    {
        char file[5];
        strcpy(file, files[i]);

        // if prev_path is empty
        // then set prev_path to files[i]
        // else
        if (prev_path == "")
        {
            strcpy(prev_path, "./");
            strcpy(prev_path, file);
        }
        else
        {
            if (prev_path[(int)strlen(prev_path) - 1] == "/")
            {
                strcpy(prev_path, file);
            }
            else
            {
                strcpy(prev_path, "/");
                strcpy(prev_path, file);
            }
        }

        printf("prev_path: %s", prev_path);

        // if isdir()
        if (check_is_dir(file) == true)
        {

            perform_on_files()
                go_through_files(1, file, prev_path, func);
        }
        else
        {
            // perform change
            func(file);
        }
    }
}

int close_program(bool success)
{
    free(PATH);
    free(IGNORED_COLOR);
    free(DESIRED_COLOR);
    return success;
}

void tmp_func(char yolo[]) {}

int main(int argc, char *argv[])
{
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

    char *file[] = {{PATH}};
    perform_on_files(1, file, "", tmp_func);

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
    return close_program(true);
}
