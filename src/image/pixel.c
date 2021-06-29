#include <stdlib.h>
#include <math.h>
#include "pixel.h"

void copy_px_to_png_bytep(rgba *source, png_bytep dest)
{
    dest[0] = source->r;
    dest[1] = source->g;
    dest[2] = source->b;
    dest[3] = source->a;
}

void copy_png_bytep_to_rgbap(png_bytep source, rgba *dest)
{
    dest->r = source[0];
    dest->g = source[1];
    dest->b = source[2];
    dest->a = source[3];
}

bool compare_near(int value, int other_value, int tolerance)
{
    return value == other_value || abs(value - other_value) <= tolerance;
}

bool compare_rgba(rgba *source, rgba *compare, int tolerance, bool include_alpha)
{
    if (
        compare_near(source->r, compare->r, tolerance) &&
        compare_near(source->g, compare->g, tolerance) &&
        compare_near(source->b, compare->b, tolerance))
    {
        if (include_alpha)
        {
            if (source->a == compare->a)
                return true;
        }
        else
        {
            return true;
        }
    }

    return false;
}

color *init_color()
{
    color *col = (color *)malloc(sizeof(color));
    col->original_color = NULL;
    col->original_hsv = NULL;
    col->mod_color = NULL;
    col->mod_hsv = NULL;
    return col;
}

void convert_original_color_to_hsv(color *col)
{
    hsv *hsvp;
    rgba *rgbap;

    if (col->mod_hsv == NULL && col->mod_color != NULL)
    {
        col->mod_hsv = (hsv *)malloc(sizeof(hsv));
        hsvp = col->mod_hsv;
        rgbap = col->mod_color;
    }
    else if (col->original_hsv == NULL && col->original_color != NULL)
    {
        col->original_hsv = (hsv *)malloc(sizeof(hsv));
        hsvp = col->original_hsv;
        rgbap = col->original_color;
    }
    else
    {
        fprintf(stderr, "COLORS EITHER EMPTY OR HSV IS ALREADY COMPUTED\n");
        abort();
    }

    rgba_to_hsv(rgbap, hsvp);
}

void rgba_to_hsv(rgba *rgbap, hsv *hsvp)
{
    double r, g, b;

    r = ((double)rgbap->r) / 255.0;
    g = ((double)rgbap->g) / 255.0;
    b = ((double)rgbap->b) / 255.0;
    // printf("%3f %3f %3f\n", r, g, b);

    int h;
    float s, v;

    double cmax = fmax(r, fmax(g, b));
    double cmin = fmin(r, fmin(g, b));
    double delta = cmax - cmin;
    double mod = 360.0;

    if (cmax == cmin)
        h = 0.0;
    else if (cmax == r)
        h = 60.0 * fmod((g - b) / delta, 6.0);
    else if (cmax == g)
        h = 60.0 * (((b - r) / delta) + 2.0);
    else if (cmax == b)
        h = 60.0 * (((r - g) / delta) + 4.0);

    if (cmax == 0)
        s = 0;
    else
        s = delta / cmax;

    // b = (cmax + cmin) / 2;
    v = cmax;
    // printf("v: %3f\n", v);

    hsvp->h = round(h);
    hsvp->s = s;
    hsvp->v = v;
    // printf("%3d %3f %3f\n\n", hsvp->h, hsvp->s, hsvp->v);
}

void convert_modified_hsv_to_rgba(color *col)
{
    hsv *hsvp;
    rgba *rgbap;
    int a;

    if (col->mod_hsv != NULL && col->mod_color == NULL)
    {
        hsvp = col->mod_hsv;
        col->mod_color = (rgba *)malloc(sizeof(rgba));
        rgbap = col->mod_color;

        a = col->original_color->a;
    }
    else if (col->original_hsv != NULL && col->original_color == NULL)
    {
        hsvp = col->original_hsv;
        col->original_color = (rgba *)malloc(sizeof(rgba));
        rgbap = col->original_color;

        a = col->mod_color->a;
    }
    else
    {
        fprintf(stderr, "RGBA IS EITHER COMPUTED ALREADY OR HSV IS EMPTY\n");
        abort();
    }

    hsv_to_rgba(hsvp, rgbap, a);
}

void hsv_to_rgba(hsv *hsv, rgba *rgba, int alpha)
{

    printf("HSV -> h: %3d s: %3f v: %3fd\n", hsv->h, hsv->s, hsv->v);

    float r, g, b;

    float h = (float)hsv->h,
          s = (float)hsv->s / 100.0,
          v = ((float)hsv->v) / 100.0;

    printf("HSV -> h: %3f s: %3f b: %3f\n", h, s, v);

    float c = round((1.0 - abs(2.0 * v - 1.0)) * s);
    float x = round(c * (1.0 - abs(fmodf((h / 60.0), 2.0)) - 1.0));
    float m = (v - (c / 2.0));

    if (h < 0.0 || h > 360.0)
        h = 0.0;

    if (h < 60.0)
    {
        printf("HERE\n");
        r = c;
        g = x;
        b = 0.0;
    }

    else if (h < 120.0)
    {
        r = x;
        g = c;
        b = 0.0;
    }

    else if (h < 180.0)
    {
        r = 0.0;
        g = c;
        b = x;
    }

    else if (h < 240.0)
    {
        r = 0.0;
        g = x;
        b = c;
    }

    else if (h < 300.0)
    {
        r = x;
        g = 0.0;
        b = c;
    }

    else if (h < 360.0)
    {
        r = c;
        g = 0.0;
        b = x;
    }

    rgba->r = (int)round((r + m) * 255.0);
    rgba->g = (int)round((g + m) * 255.0);
    rgba->b = (int)round((b + m) * 255.0);
    rgba->a = alpha;

    printf("RGBA -> r: %3d g: %3d b: %3d a: %3d\n\n", rgba->r, rgba->g, rgba->b, rgba->a);
}