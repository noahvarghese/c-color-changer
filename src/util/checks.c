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

bool check_is_png(FILE *file) {
    if ( file == NULL ) {
        return false;
    }

    unsigned char bytes[8];
    fread(bytes, 8, 1, file);

    if (
        bytes[0] == 137 &&
        bytes[1] == 80 &&
        bytes[2] == 78 &&
        bytes[3] == 71 &&
        bytes[4] == 13 &&
        bytes[5] == 10 &&
        bytes[6] == 26 &&
        bytes[7] == 10
    ) {
        return true;
    }

    return false;
}

bool check_is_jpg(FILE *file) {
    if ( file == NULL ) {
        return false;
    }

    unsigned char bytes[3];
    fread(bytes, 3, 1, file);

    if (
        bytes[0] == 0xff &&
        bytes[1] == 0xd8 &&
        bytes[2] == 0xff
    ) {
        return true;
    }

    return false;
}