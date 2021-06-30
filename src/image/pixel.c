#include <stdlib.h>
#include <math.h>
#include "pixel.h"

// Handles all pixel related data structures
// stuff like RGBA and HSV implmentations and calculations

void free_color(color *col) {
    if (col != NULL) {
        if (col->original_color != NULL) {
            free(col->original_color);
            col->original_color = NULL;
        }

        if (col->original_hsv != NULL) {
            free(col->original_hsv);
            col->original_hsv = NULL;
        }

        if (col->mod_color != NULL) {
            free(col->mod_color);
            col->mod_color = NULL;
        }

        if (col->mod_hsv != NULL) {
            free(col->mod_hsv);
            col->mod_hsv = NULL;
        } 
        free(col);
        col = NULL;
    }
}

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

    v = cmax;

    hsvp->h = ceil(h);
    hsvp->s = s;
    hsvp->v = v;
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
    double r, g, b;

    double h = (double)hsv->h, s = hsv->s, v = hsv->v;

    double c = v * s;
    double x = c * (1.0 - fabs(fmod((h / 60.0), 2.0) - 1.0));
    double m = v - c;

    if (h < 0.0 || h > 360.0)
        h = 0.0;

    if (h < 60.0)
    {
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

    rgba->r = (int)ceil((r + m) * 255.0);
    rgba->g = (int)ceil((g + m) * 255.0);
    rgba->b = (int)ceil((b + m) * 255.0);
    rgba->a = alpha;
}