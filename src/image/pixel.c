#include "pixel.h"

bool is_transparent(png_bytep px)
{
    if (px[3] == 0)
        return true;

    return false;
}

bool png_byte_is_equal(png_bytep origin_px, int *compare_px)
{

    if (
        origin_px[0] == compare_px[0] &&
        origin_px[1] == compare_px[1] &&
        origin_px[2] == compare_px[2] &&
        origin_px[3] == compare_px[3])
    {
        return true;
    }

    return false;
}

void copy_pixel(png_bytep dest, int *src)
{
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
}