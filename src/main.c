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
#include "main.h"

struct split_path {
    char **files;
    int files_count;
};

program_vars *vars;

struct split_path get_files_from_string(char files_string[], const char files_delim)
{
    struct split_path paths;
    paths.files_count = 0;
    paths.files = 0;

    char delim[] = {files_delim, 0};
    char *tmp = files_string;
    char *last_comma = 0;

    while (*tmp) {
        if ( files_delim == *tmp) {
            paths.files_count++;
            last_comma = tmp;
        }

        tmp++;
    }

    /* Add space for trailing token. */
    paths.files_count += last_comma < (files_string + strlen(files_string) - 1);

    paths.files = malloc(sizeof(char *) * paths.files_count);

    if ( paths.files ) {
        char* rest = files_string;

        for ( int i = 0; i <= paths.files_count; i++ ) {
            char *token = strtok_r(files_string, delim, &rest);

            if (token) {
                assert(i <= paths.files_count);
                *(paths.files + i) = strdup(token);
                token = strtok_r(files_string, delim, &rest);
            }

            if (i == paths.files_count) {
                *(paths.files + i) = 0;
            }
        }
    }

    return paths;
}

char** append_to_array(char **dest, int count, char* string) {
    printf("Count: %d\n", count);
    printf("New: %s\n", string);

    char **tmp_array;
    size_t size = sizeof(dest) + sizeof(string);

    printf("Size: %d\n", size);
    tmp_array = malloc(size);

    for ( int i = 0; i < count - 1; i++) {
        printf("%s\n", *(dest + i));
        *(tmp_array + i) = *(dest + i);
        printf("Current: %s\n", *(dest + i));
    } 

    tmp_array[count - 1] = string;
    // memcpy(dest, tmp_array, count * sizeof(char));
    // dest = tmp_array;

    // printf("Dest 0 %s\n", dest[0]);
    // printf("tmp 0 %s\n", tmp_array[0]);
    return tmp_array;
}

struct split_path read_dir(char* dirname) {
    struct dirent *dp;
    DIR *dfd;
    struct split_path files;

    if ((dfd = opendir(dirname)) == NULL) {
        printf("Can't open %s\n", dirname);
        return files;
    }

    files.files = 0;
    files.files_count = 0;

    while((dp = readdir(dfd)) != NULL) {
        printf("DP NAME: %s\n", dp->d_name);
        if ( dp->d_name[0] != '.' ) {
            char *file = malloc(strlen(dp->d_name) + sizeof(char));
            memcpy(file, dp->d_name, strlen(dp->d_name) + sizeof(char));

            if (strcmp(file, ".") != 0 && strcmp(file, "..") != 0) {
                printf("File: %s\n", file);
                files.files_count++;
                files.files = append_to_array(files.files, files.files_count, file);
                // printf("New File: %s\n", files.files[0]);
            }
        }
    }

    return files;
}

void perform_on_files(struct split_path files_struct, char prev_path[], void (*func)(char[]))
{
    // Loop over files
    for (int i = 0; i < files_struct.files_count; i++)
    {
        printf("Files: %s\n", files_struct.files[i]);
        // char *file = strdup(files_struct.files[i]);

        char *file = malloc(strlen(files_struct.files[i]) * sizeof(char));
        memcpy(file, *(files_struct.files+ i), strlen(*(files_struct.files + i)) * sizeof(char));

        if ( check_is_dir(file) == false ) {
            printf("This is not a directory %s\n", file);
            if ( check_is_file(file) == true ) {
                printf("This is not a file %s\n", file);
                func(file);
            }
            return;
        }

        if ( strlen(file) == 1 && file[0] == '.' )
            return;


        if (prev_path == "")
        {
            const char first_el = file[0];
            const char second_el = file[1];

            // create relative path if not already there
            if ( first_el != '.' && second_el != '/' ) {
                prev_path = strdup("./");
                strcat(prev_path, file);
                // strcpy(prev_path, "./");
            } 
            else {
                prev_path = strdup(file);
            }
            // strcpy(prev_path, file);
        }
        else
        {
            // check if there is a trailing backslash
            // and adds the file path
            if (prev_path[(int)strlen(prev_path) - 1] == '/')
            {
                strcat(prev_path, file);
                // strcpy(prev_path, file);
            }
            else
            {
                strcat(prev_path, "/");
                strcat(prev_path, file);
                // strcpy(prev_path, "/");
                // strcpy(prev_path, file);
            }
        }

        printf("prev_path: %s\n", prev_path);

        // get struct of files
        // pass to perform on files
        struct split_path tmp_file_struct = read_dir(file);
        printf("File count: %d\n", tmp_file_struct.files_count);
        perform_on_files(tmp_file_struct, prev_path, func);
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
    struct split_path files_struct = get_files_from_string(vars->path, delim);

    perform_on_files(files_struct, "", tmp_func);

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
