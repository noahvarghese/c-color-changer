#include <stdlib.h>
#include "image.h"
#include "pixel.h"
#include "../linked_list/color_node.h"
#include "png.h"
#include "../main.h"

// Handles all png manipulations (read/write/modify/get stats)
void free_png(image_png *png)
{
    if( png != NULL) {
        if (png->name != NULL) {
            free(png->name);
            png->name = NULL;
        }

        if (png->new_name != NULL) {
            free(png->new_name);
            png->new_name = NULL;
        }
        
        if (png->rows != NULL) {
            free(png->rows);
            png->rows = NULL;
        }

        free(png);
        png = NULL;
    }
}

image_png *init_png_image(char *name)
{
    image_png *png = (image_png *)malloc(sizeof(image_png));
    png->name = name;
    png->new_name = modify_image_name(name);
    png->height = 0;
    png->width = 0;
    png->color_type = 0;
    png->bit_depth = 0;
    png->rows = NULL;
    return png;
}

void write_png(image_png *png_image)
{
    int y;

    FILE *file = fopen(png_image->new_name, "wb");

    if (!file)
        abort();

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png)
        abort();

    png_infop info = png_create_info_struct(png);

    if (!info)
        abort();

    if (setjmp(png_jmpbuf(png)))
        abort();

    png_init_io(png, file);

    png_set_IHDR(
        png,
        info,
        png_image->width,
        png_image->height,
        png_image->bit_depth,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png, info);

    if (!png_image->rows)
        abort();

    png_write_image(png, png_image->rows);
    png_write_end(png, NULL);

    for (int i = 0; i < png_image->height; i++)
    {
        free(png_image->rows[i]);
    }

    fclose(file);
    png_destroy_write_struct(&png, &info);
}

void read_png(image_png *png_image)
{
    printf("File: %s\n", png_image->name);

    FILE *file = fopen(png_image->name, "r");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png)
        exit(EXIT_FAILURE);

    png_infop info = png_create_info_struct(png);

    if (!info)
        exit(EXIT_FAILURE);

    if (setjmp(png_jmpbuf(png)))
        exit(EXIT_FAILURE);

    png_init_io(png, file);

    png_read_info(png, info);

    png_image->width = png_get_image_width(png, info);
    png_image->height = png_get_image_height(png, info);
    png_image->color_type = png_get_color_type(png, info);
    png_image->bit_depth = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    if (png_image->bit_depth == 16)
        png_set_strip_16(png);

    if (png_image->color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if (png_image->color_type == PNG_COLOR_TYPE_GRAY && png_image->bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if (png_image->color_type == PNG_COLOR_TYPE_RGB ||
        png_image->color_type == PNG_COLOR_TYPE_GRAY ||
        png_image->color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if (png_image->color_type == PNG_COLOR_TYPE_GRAY ||
        png_image->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    // if (png_image->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
    //     printf("YAY THIS HAS ALPHA\n");

    png_read_update_info(png, info);

    if (png_image->rows)
        abort();

    png_image->rows = (png_bytep *)malloc(sizeof(png_bytep) * png_image->height);
    for (int y = 0; y < png_image->height; y++)
    {
        png_image->rows[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, png_image->rows);

    fclose(file);

    png_destroy_read_struct(&png, &info, NULL);
}

void png_stats(image_png *png, color_ll *cll, int tolerance)
{
    // get number of different colors that are not the ignored_color, use linked list (make one for pixel arrays),
    for (int y = 0; y < png->height; y++)
    {
        png_bytep row = png->rows[y];
        for (int x = 0; x < png->width; x++)
        {
            png_bytep px = &(row[x * 4]);
            rgba *rgb = (rgba *)malloc(sizeof(rgba));
            copy_png_bytep_to_rgbap(px, rgb);

            if (!compare_rgba(rgb, vars->ignored_rgba, tolerance, false) && px[3] != 0)
            {
                if (!png_bytep_exists_in_cll(cll, px, tolerance))
                    append_data_to_cll(cll, px);
            }
        }
    }
}

void modify_png(image_png *png, color_ll *cll, int tolerance)
{
    // amendment
    // assumes cll is sorted with the first node being the brightest
    // get color_node and index
    // hsv of replacement color is the same as hsv of desired_color but the v = v - (index * 5)

    for (int y = 0; y < png->height; y++)
    {
        png_bytep row = png->rows[y];
        for (int x = 0; x < png->width; x++)
        {
            png_bytep px = &(row[x * 4]);
            rgba *rgb = (rgba *)malloc(sizeof(rgba));
            copy_png_bytep_to_rgbap(px, rgb);

            if (!compare_rgba(rgb, vars->ignored_rgba, tolerance, false) && px[3] != 0)
            {
                c_node *node = find_by_original_color(cll, rgb, tolerance);

                if (node == NULL)
                {
                    continue;
                }

                // printf("%3d %3d %3d %3d\n", node->color->mod_color->r, node->color->mod_color->g, node->color->mod_color->b, px[3]);
                px[0] = node->color->mod_color->r;
                px[1] = node->color->mod_color->g;
                px[2] = node->color->mod_color->b;

                // printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x + 1, y + 1, px[0], px[1], px[2], px[3]);
                // int *hsb = intp_to_hsb(vars->desired);
                // figure out how many steps of hue to change
                // modify h in hsb
                // int *rgba = hsb_to_intp(hsb);
                // copy_png_bytep_to_rgbap(px, node->color->mod_color->px.i);
            }
        }
    }
}
