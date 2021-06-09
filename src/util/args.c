#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "checks.h"
#include "../main.h"


#include "args.h"

bool parse_args(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        size_t length = strlen(argv[i]);

        char arg[length];
        strcpy(arg, argv[i]);

        if (i == argc)
        {
            printf("Missing an argument\n");
            return 1;
        }

        if (strcmp(arg, "-i") == 0 || strcmp(arg, "--ignore") == 0)
        {
            if (check_color_format(argv[++i]) == true)
            {
                vars->ignored_color = argv[i];
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
                vars->desired_color = argv[i];
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
                    vars->is_dir = false;
                }
                else
                {
                    if (check_is_dir(argv[i]) == true)
                    {
                        vars->is_dir = true;
                    }
                    else
                    {
                        printf(PATH_NMATCH_MESSAGE);
                        return false;
                    }
                }
                vars->path = argv[i];
            }
            else
            {
                printf(PATH_NEXISTS_MESSAGE);
                return false;
            }
        }
    }

    if ( vars->ignored_color == NULL )
        return false;
    
    if (vars->desired_color == NULL )
        return false;

    if (vars->path == NULL)
        return false;

    return true;
}