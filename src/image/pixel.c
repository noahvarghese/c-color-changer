#include <stdlib.h>
#include "pixel.h"

bool intp_is_transparent(int *px)
{
    if (px[3] == 0)
        return true;

    return false;
}

bool png_bytep_is_transparent(png_bytep px)
{
    if (px[3] == 0)
        return true;

    return false;
}

bool intp_is_equal(int *origin, int *compare)
{
    if (
        origin[0] == compare[0] &&
        origin[1] == compare[1] &&
        origin[2] == compare[2] &&
        origin[3] == compare[3])
    {
        return true;
    }

    return false;
}

bool compare_near(int value, int other_value, int tolerance)
{
    return value == other_value || abs(value - other_value) <= tolerance;
}

bool png_bytep_is_equal(png_bytep origin_px, int *compare_px, int tolerance)
{
    if (
        compare_near(origin_px[0], compare_px[0], tolerance) &&
        compare_near(origin_px[1], compare_px[1], tolerance) &&
        compare_near(origin_px[2], compare_px[2], tolerance))
    {
        return true;
    }
    // if (
    //     origin_px[0] == compare_px[0] &&
    //     origin_px[1] == compare_px[1] &&
    //     origin_px[2] == compare_px[2] &&
    //     origin_px[3] == compare_px[3])
    // {
    //     return true;
    // }

    return false;
}

void copy_to_intp(int *dest, png_bytep src)
{
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
}

void copy_to_png_bytep(png_bytep dest, int *src)
{
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
}

png_bytep png_bytep_from_intp(int *px)
{
    png_bytep new_px = (png_bytep)malloc(sizeof(png_byte));
    copy_to_png_bytep(new_px, px);
    return new_px;
}

int *intp_from_png_bytep(png_bytep px)
{
    int *new_px = (int *)malloc(sizeof(int) * 4);
    copy_to_intp(new_px, px);
    return new_px;
}