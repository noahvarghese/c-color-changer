#include <stdlib.h>
#include <math.h>
#include "pixel.h"

void copy_to_px(png_bytep px, int *rgba)
{
    px[0] = rgba[0];
    px[1] = rgba[1];
    px[2] = rgba[2];
    px[3] = rgba[3];
}

bool compare_near(int value, int other_value, int tolerance)
{
    return value == other_value || abs(value - other_value) <= tolerance;
}

bool rgba_is_equal(png_bytep image_px, int *node_px, int tolerance)
{
    if (
        compare_near(image_px[0], node_px[0], tolerance) &&
        compare_near(image_px[1], node_px[1], tolerance) &&
        compare_near(image_px[2], node_px[2], tolerance))
    {
        return true;
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

void rgba_to_hsv(color *col)
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

    double r, g, b;

    if (rgbap->type == INT)
    {
        r = ((double)rgbap->px.i[0]) / 255.0;
        g = ((double)rgbap->px.i[1]) / 255.0;
        b = ((double)rgbap->px.i[2]) / 255.0;
        // printf("%3d %3d %3d\n", rgbap->px.i[0], rgbap->px.i[1], rgbap->px.i[2]);
    }
    else
    {
        r = ((double)rgbap->px.p[0]) / 255.0;
        g = ((double)rgbap->px.p[1]) / 255.0;
        b = ((double)rgbap->px.p[2]) / 255.0;
        // printf("%3d %3d %3d\n", rgbap->px.p[0], rgbap->px.p[1],rgbap->px.p[2]);
    }
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

int get_alpha(rgba *rgbap)
{
    if (rgbap->type == INT)
    {
        return rgbap->px.i[3];
    }
    else
    {
        return rgbap->px.p[3];
    }
}

void hsv_to_rgba(color *col)
{
    hsv *hsvp;
    rgba *rgbap;
    int a;

    if (col->mod_hsv != NULL && col->mod_color == NULL)
    {
        hsvp = col->mod_hsv;
        col->mod_color = (rgba *)malloc(sizeof(rgba));
        rgbap = col->mod_color;

        a = get_alpha(col->original_color);
    }
    else if (col->original_hsv != NULL && col->original_color == NULL)
    {
        hsvp = col->original_hsv;
        col->original_color = (rgba *)malloc(sizeof(rgba));
        rgbap = col->original_color;

        a = get_alpha(col->mod_color);
    }
    else
    {
        fprintf(stderr, "RGBA IS EITHER COMPUTED ALREADY OR HSV IS EMPTY\n");
        abort();
    }

    printf("HSV -> h: %3d s: %3f v: %3fd\n", hsvp->h, hsvp->s, hsvp->v);

    float r, g, b;

    float h = (float)hsvp->h,
          s = (float)hsvp->s / 100.0,
          v = ((float)hsvp->v) / 100.0;

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

    rgbap->type = INT;
    rgbap->px.i = (int *)malloc(sizeof(int) * 4);

    rgbap->px.i[0] = (int)round((r + m) * 255.0);
    rgbap->px.i[1] = (int)round((g + m) * 255.0);
    rgbap->px.i[2] = (int)round((b + m) * 255.0);
    rgbap->px.i[3] = a;

    printf("RGBA -> r: %3d g: %3d b: %3d a: %3d\n\n", rgbap->px.i[0], rgbap->px.i[1], rgbap->px.i[2], rgbap->px.i[3]);
}