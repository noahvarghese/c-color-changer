#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "checks.h"
#include "str.h"
#include "../main.h"

#include "args.h"

bool parse_args(int argc, char *argv[])
{
    vars->tolerance = -1;

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
                vars->ignored_hex = argv[i];
                vars->ignored_rgba = hex_from_string(vars->ignored_hex);
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
                vars->mod_hex = argv[i];
                vars->mod_rgba = hex_from_string(vars->mod_hex);
                vars->mod_hsv = (hsv *)malloc(sizeof(hsv));
                rgba_to_hsv(vars->mod_rgba, vars->mod_hsv);
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
        else if (strcmp(arg, "-t") == 0 || strcmp(arg, "--tolerance") == 0) {
            int tolerance = atoi(argv[++i]);

            if (tolerance > -1)
                vars->tolerance = tolerance;
        }
    }

    if (vars->ignored_rgba == NULL)
    {
        fprintf(stderr, "Ignored color not set\n");
        exit(EXIT_FAILURE);
    }

    if (vars->mod_rgba == NULL)
    {
        fprintf(stderr, "Desired color not set\n");
        exit(EXIT_FAILURE);
    }

    if (vars->path == NULL)
    {
        fprintf(stderr, "Path to file(s) not set\n");
        exit(EXIT_FAILURE);
    }

    if (vars->tolerance == -1)  {
        fprintf(stderr, "Tolerance must be an integer greater than 0\n");
        exit(EXIT_FAILURE);
    }

    return true;
}