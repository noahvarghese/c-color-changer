#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
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
            fprintf(stderr, "Missing an argument\n");
            exit(EXIT_FAILURE);
        }

        if (strcmp(arg, "-i") == 0 || strcmp(arg, "--ignore") == 0)
        {
            if (check_color_format(argv[++i]) == true)
            {
                vars->ignored_color = argv[i];
            }
            else
            {
                fprintf(stderr, IGNORED_MESSAGE);
                exit(EXIT_FAILURE);
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
                fprintf(stderr, COLOR_MESSAGE);
                exit(EXIT_FAILURE);
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
                        fprintf(stderr, PATH_NMATCH_MESSAGE);
                        exit(EXIT_FAILURE);
                    }
                }
                vars->path = argv[i];
            }
            else
            {
                fprintf(stderr, PATH_NEXISTS_MESSAGE);
                exit(EXIT_FAILURE);
            }
        }
    }

    if (vars->ignored_color == NULL)
    {
        fprintf(stderr, "Ignored color not set\n");
        exit(EXIT_FAILURE);
    }

    if (vars->desired_color == NULL)
    {
        fprintf(stderr, "Desired color not set\n");
        exit(EXIT_FAILURE);
    }

    if (vars->path == NULL)
    {
        fprintf(stderr, "Path to file(s) not set\n");
        exit(EXIT_FAILURE);
    }

    return true;
}