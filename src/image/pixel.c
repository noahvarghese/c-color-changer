#include <stdlib.h>
#include <math.h>
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

int *png_bytep_to_hsb(png_bytep px)
{
    int *hsb = (int *)malloc(sizeof(int) * 4);

    // make ranger from 0 - 1
    double r = px[0] / 255,
           g = px[1] / 255,
           b = px[2] / 255,
           a = px[3];

    double h, s, l;

    double cmax = fmax(r, fmax(g, b));
    double cmin = fmin(r, fmin(g, b));
    double delta = cmax - cmin;
    double mod = 360;

    if (cmax == cmin)
        h = 0;
    else if (cmax == r)
        h = fmod(60 * ((g - b) / delta) + 360, mod);
    else if (cmax == g)
        h = fmod(60 * ((b - r) / delta) + 120, mod);
    else if (cmax == b)
        h = fmod(60 * ((r - g) / delta) + 240, mod);

    if (delta == 0)
        s = 0;
    else
        s = (delta / cmax) * 100;

    b = (cmax + cmin) / 2;

    hsb[0] = round(h);
    hsb[1] = round(s);
    hsb[2] = round(b);
    hsb[3] = px[3];

    return hsb;
}

int *intp_to_hsb(int *px)
{
    printf("RGBA -> r: %3d g: %3d b: %3d a: %3d\n", px[0], px[1], px[2], px[3]);

    int *hsl = (int *)malloc(sizeof(int) * 4);

    // make ranger from 0 - 1
    double r = (double)px[0] / 255,
           g = (double)px[1] / 255,
           b = (double)px[2] / 255,
           a = px[3];

    double h, s, l;

    double cmax = fmax(r, fmax(g, b));
    double cmin = fmin(r, fmin(g, b));
    double delta = cmax - cmin;
    double mod = 360;

    if (cmax == cmin)
        h = 0;
    else if (cmax == r)
        h = fmod((double)(60 * ((g - b) / delta) + 360), mod);
    else if (cmax == g)
        h = fmod(60 * ((b - r) / delta) + 120, mod);
    else if (cmax == b)
        h = fmod((double)(60 * ((r - g) / delta) + 240), mod);

    if (delta == 0)
        s = 0;

    else
        s = (delta / cmax) * 100;

    l = ((cmax + cmin) / 2) * 100;

    hsl[0] = (int)round(h);
    hsl[1] = (int)round(s);
    hsl[2] = (int)round(l);
    hsl[3] = a;

    printf("HSLA -> h: %3d s: %3d l: %3d a: %3d\n", hsl[0], hsl[1], hsl[2], hsl[3]);
    return hsl;
}

png_bytep hsb_to_png_bytep(int *hsb)
{
}

// THIS IS HSL NOT HSV
int *hsb_to_intp(int *hsb)
{
    printf("HSBA -> h: %3d s: %3d b: %3d a: %3d\n", hsb[0], hsb[1], hsb[2], hsb[3]);
    int r, g, b;
    float _r, _g, _b;

    float h = (float)hsb[0],
          s = (float)hsb[1] / 100.0,
          l = ((float)hsb[2]) / 100.0,
          a = (float)hsb[3];

    printf("HSBA -> h: %3f s: %3f b: %3f a: %3f\n", h, s, l, hsb[3]);

    float c = round((1.0 - abs(2.0 * l - 1.0)) * s);
    float x = round(c * (1.0 - abs(fmodf((h / 60.0), 2.0)) - 1.0));
    float m = (l - (c / 2.0));

    if (h < 0.0 || h > 360.0)
        h = 0.0;

    if (h < 60.0)
    {
        printf("HERE\n");
        _r = c;
        _g = x;
        _b = 0.0;
    }

    else if (h < 120.0)
    {
        _r = x;
        _g = c;
        _b = 0.0;
    }

    else if (h < 180.0)
    {
        _r = 0.0;
        _g = c;
        _b = x;
    }

    else if (h < 240.0)
    {
        _r = 0.0;
        _g = x;
        _b = c;
    }

    else if (h < 300.0)
    {
        _r = x;
        _g = 0.0;
        _b = c;
    }

    else if (h < 360.0)
    {
        _r = c;
        _g = 0.0;
        _b = x;
    }

    r = (int)((_r + m) * 255.0);
    g = (int)((_g + m) * 255.0);
    b = (int)((_b + m) * 255.0);

    // double hh, p, q, t, ff;
    // long i;

    // hh = hsb[0];
    // if (hh >= 360.0)
    //     hh = 0.0;

    // hh /= 60.0;

    // i = (long)hh;
    // ff = hh - i;
    // p = hsb[2] * (1.0 - hsb[1]);
    // q = hsb[2] * (1.0 - (hsb[1] * ff));
    // t = hsb[2] * (1.0 - (hsb[2] * (1.0 - ff)));

    // printf("I: %d\n", i);
    // switch (i)
    // {
    // case 0:
    //     r = hsb[2];
    //     g = t;
    //     b = p;
    //     break;
    // case 1:
    //     r = q;
    //     g = hsb[2];
    //     b = p;
    //     break;
    // case 2:
    //     r = p;
    //     g = hsb[2];
    //     b = t;
    //     break;

    // case 3:
    //     r = p;
    //     g = q;
    //     b = hsb[2];
    //     break;
    // case 4:
    //     r = t;
    //     g = p;
    //     b = hsb[2];
    //     break;
    // case 5:
    // default:
    //     r = hsb[2];
    //     g = p;
    //     b = q;
    //     break;
    // }

    int *rgba = (int *)malloc(sizeof(int) * 4);
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = a;
    printf("RGBA -> r: %3d g: %3d b: %3d a: %3d\n\n", rgba[0], rgba[1], rgba[2], rgba[3]);
    return rgba;
}