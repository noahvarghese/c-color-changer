#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include "image.h"

image_png *init_png_image(char *name)
{
    image_png *png = (image_png *)malloc(sizeof(image_png));
    png->name = name;
    png->height = 0;
    png->width = 0;
    png->png = NULL;
    png->rows = NULL;
    png->info = NULL;
    return png;
}

void parse_png(image_png *png_image)
{
    printf("File: %s\n", png_image->name);
    int width, height;
    png_byte color_type, bit_depth;
    png_bytep *rows = NULL;

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

    width = png_get_image_width(png, info);
    height = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    if (bit_depth == 16)
        png_set_strip_16(png);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if (color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        printf("YAY THIS HAS ALPHA");

    png_read_update_info(png, info);

    if (rows)
        abort();

    rows = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++)
    {
        rows[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, rows);

    fclose(file);

    png_destroy_read_struct(&png, &info, NULL);
}

image *init_image(char *file_name)
{
    image *img = (image *)malloc(sizeof(image));
    img->data = NULL;
    img->file_name = file_name;
    img->data_length = 0;
    img->width = 0;
    img->height = 0;
    return img;
}

// Will close the file
image *get_contents(char *file_name)
{
    image *img = init_image(file_name);

    FILE *file = fopen(file_name, "r");
    fseek(file, 0, SEEK_END);

    img->data_length = ftell(file);

    rewind(file);

    img->data = calloc(1, img->data_length + 1);

    if (!img->data)
    {
        fclose(file);
        fprintf(stderr, "Memory allocation failed for file: %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    int status = fread(img->data, img->data_length, 1, file);

    if (status != 1)
    {
        fclose(file);
        fprintf(stderr, "Entire read faile: %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return img;
}

void png_dimensions(image *img)
{
    char width[4] = {img->data[16], img->data[17], img->data[18], img->data[19]};
    char height[4] = {img->data[20], img->data[21], img->data[22], img->data[23]};
    // FILE *file = fopen(img->file_name, "r");

    // fseek(file, 16, SEEK_SET);
    // // Bytes 16-20 are width
    // fread((char *)&img->width, 4, 1, file);

    // // Bytes 20-24 are height
    // fseek(file, 20, SEEK_SET);
    // fread((char *)&img->height, 4, 1, file);

    // Gotta keep in mind the endianess
    img->width = ntohl(*(int *)width);
    img->height = ntohl(*(int *)height);
}

void jpg_dimensions(image *img)
{
    // starting byte (start of the first block)
    int i = 0;

    // unsigned short block_length = img->data[i] * 256 + img->data[i + 1];
    // printf("Block size: %hu\n", block_length);

    printf("Data size: %d\n", img->data_length);
    while (i < img->data_length)
    {
        // i += block_length;
        // printf("I: %d\n", i);

        // // protect against segmentation faults
        // if (i >= img->data_length)
        // {
        //     printf("Saved from seg\n");
        //     return;
        // }

        // // checks if we are actually at the start of another block
        // printf("Compare %d %d\n", img->data[i + 1], 0xff);
        // if (img->data[i] != 0xff)
        // {
        // printf("Not start of block\n");
        // return;
        // }

        if (img->data[i] == 0xff && img->data[i + 1] == 0xc0)
        {
            printf("YAY\n");
            img->width = img->data[i + 7] + img->data[i + 8];
            img->height = img->data[i + 5] + img->data[i + 6];
            return;
        }
        // else
        // {
        // printf("Next\n");
        // i += 2;
        // block_length = img->data[i] * 256 + img->data[i + 1];
        // }

        i++;
    }
    return;
}