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

    return false;
}

void rgba_to_hsv(color *col)
{
    hsv *hsvp;
    rgba *rgbap;

    if (col->mod_hsv == NULL && col->mod_color != NULL)
    {
        hsvp = col->mod_hsv;
        rgbap = col->mod_color;
    }
    else if (col->original_hsv == NULL && col->original_color != NULL)
    {
        hsvp = col->original_hsv;
        rgbap = col->original_color;
    }
    else
    {
        fprintf(stderr, "COLORS EIThER EMPTY OR HSV IS ALREADY COMPUTED\n");
        abort();
    }

    int r, g, b;

    if (rgbap->type == INT)
    {
        r = rgbap->px.i[0];
        g = rgbap->px.i[1];
        b = rgbap->px.i[2];
    }
    else
    {
        r = rgbap->px.p[0];
        g = rgbap->px.p[1];
        b = rgbap->px.p[2];
    }

    r /= 255;
    g /= 255;
    b /= 255;

    int h;
    float s, v;

    double cmax = fmax(r, fmax(g, b));
    double cmin = fmin(r, fmin(g, b));
    double delta = cmax - cmin;
    double mod = 360;

    if (cmax == cmin)
        h = 0;
    else if (cmax == r)
        h = 60 * fmod((g - b) / delta, 6);
    else if (cmax == g)
        h = 60 * (((b - r) / delta) + 2);
    else if (cmax == b)
        h = 60 * (((r - g) / delta) + 4);

    if (cmax == 0)
        s = 0;
    else
        s = delta / cmax;

    // b = (cmax + cmin) / 2;
    v = cmax;

    hsvp->h = round(h);
    hsvp->s = round(s);
    hsvp->v = round(v);
}

// THIS IS HSL NOT HSV
void *hsv_to_rgba(color *col)
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