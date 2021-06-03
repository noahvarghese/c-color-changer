#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <sys/stat.h>
#include <sys/types.h>

char *IGNORED_COLOR;
char *DESIRED_COLOR;
char *PATH;
bool IS_DIR;

void parse_files(char *file_list[], char prev_path[])
{
}

bool check_file_exists(const char *filename)
{
    FILE *file;
    if (file = fopen(filename, "r"))
    {
        fclose(file);
        return true;
    }
    return false;
}

bool check_is_file(const char *filename)
{
    struct stat statbuf;

    stat(filename, &statbuf);

    if (S_ISREG(statbuf.st_mode) != 0)
        return true;
    else
        return false;
}

bool check_is_dir(const char *filename)
{
    struct stat statbuf;

    stat(filename, &statbuf);
    if (S_ISDIR(statbuf.st_mode) != 0)
        return true;
    else
        return false;
}

int check_color_format(char color[])
{
    int length = strlen(color);
    int start = ((int)color[0]) == '#' ? 1 : 0;

    // uppercase string
    for (int i = start; i < length; i++)
    {
        if (color[i] >= 'a' && color[i] <= 'z')
        {
            color[i] = color[i] - 32;
        }

        if ((color[i] < '0' || color[i] > '9') && (color[i] < 'A' || color[i] > 'F'))
        {
            return false;
        }
    }

    return true;
}

bool parse_args(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        size_t length = strlen(argv[i]);

        char arg[length];
        strcpy(arg, argv[i]);

        if (i == argc - 1)
        {
            printf("Missing an argument\n");
            return 1;
        }

        if (strcmp(arg, "-i") == 0 || strcmp(arg, "--ignore") == 0)
        {
            if (check_color_format(argv[++i]) == true)
            {
                IGNORED_COLOR = argv[i];
            }
            else
            {
                printf("Ignored color not in correct format\n");
                return false;
            };
        }
        else if (strcmp(arg, "-c") == 0 || strcmp(arg, "--color") == 0)
        {
            if (check_color_format(argv[++i]) == true)
            {
                DESIRED_COLOR = argv[i];
            }
            else
            {
                printf("Desired color not in correct format\n");
                return false;
            }
        }
        else if (strcmp(arg, "-p") == 0 || strcmp(arg, "--path") == 0)
        {
            if (check_file_exists(argv[++i]) == true)
            {
                if (check_is_file(argv[i]) == true)
                {
                    IS_DIR = false;
                }
                else
                {
                    if (check_is_dir(argv[i]) == true)
                    {
                        IS_DIR = true;
                    }
                    else
                    {
                        printf("File is not regular file or directory.\n");
                        return false;
                    }
                }
                PATH = argv[i];
            }
            else
            {
                printf("File path does not exist\n");
                return false;
            }
        }
    }
    printf("IGNORED_COLOR: %s\n", IGNORED_COLOR);
    printf("DESIRED_COLOR: %s\n", DESIRED_COLOR);
    printf("PATH: %s\n", PATH);
    printf("IS_DIR: %d\n", IS_DIR);

    return true;
}

int main(int argc, char *argv[])
{
    // Parse cli args
    // -i/--ignore ignored color
    // -c/--color desired color
    // -p/--path image path (directory or file)

    if (parse_args(argc, argv) == true)
    {
        // Calculate 3 colors (decrease the 'hue'?) by a set amount from the desired color
        return true;
    }

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

    return false;
}
