#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "checks.h"

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

bool check_color_format(char *color)
{
    int length = strlen(color);

    if (length != 8)
    {
        printf("Color must be in rgba format without the hashtag (#)");
        return false;
    }

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

FILE *open_file_to_read(char *file_name)
{
    FILE *file = fopen(file_name, "r");

    if (file == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", file_name);
    }

    return file;
}

bool check_is_png(char *file_name)
{
    FILE *file = open_file_to_read(file_name);

    if (file == NULL)
    {
        return false;
    }
    unsigned char bytes[4];
    fread(bytes, 4, 1, file);
    fclose(file);

    if (
        bytes[0] == 137 &&
        bytes[1] == 80 &&
        bytes[2] == 78 &&
        bytes[3] == 71 /*&&*/
        // bytes[4] == 13 &&
        // bytes[5] == 10 &&
        // bytes[6] == 26 &&
        // bytes[7] == 10
    )
    {
        return true;
    }

    return false;
}

bool check_is_jpg(char *file_name)
{
    FILE *file = open_file_to_read(file_name);

    if (file == NULL)
    {
        return false;
    }

    unsigned char bytes[4];
    fread(bytes, 4, 1, file);
    fclose(file);

    if (
        bytes[0] == 0xff &&
        bytes[1] == 0xd8 &&
        bytes[2] == 0xff &&
        bytes[3] == 0xe0)
    {

        return true;
    }

    return false;
}