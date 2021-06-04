#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "args.h"
#include "checks.h"
#include "vars.h"

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
                printf(IGNORED_MESSAGE);
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
                printf(COLOR_MESSAGE);
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
                        printf(PATH_NMATCH_MESSAGE);
                        return false;
                    }
                }
                PATH = argv[i];
            }
            else
            {
                printf(PATH_NEXISTS_MESSAGE);
                return false;
            }
        }
    }
        
    printf("IGNORED_COLOR: %s\n", IGNORED_COLOR);
    printf("DESIRED_COLOR: %s\n", DESIRED_COLOR);
    printf("PATH: %s\n", PATH);
    printf("IS_DIR: %d\n", IS_DIR);

    if ( IGNORED_COLOR == NULL )
        return false;
    
    if (DESIRED_COLOR == NULL )
        return false;

    if (PATH == NULL)
        return false;

    return true;
}