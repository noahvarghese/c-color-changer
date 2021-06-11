#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include "image.h"
#include "../main.h"

void modify_png(image_png *png)
{
    for (int y = 0; y < png->height; y++)
    {
        png_bytep row = png->rows[y];
        for (int x = 0; x < png->width; x++)
        {
            png_bytep px = &(row[x * 4]);
            if (
                px[0] != vars->ignored[0] &&
                px[1] != vars->ignored[1] &&
                px[2] != vars->ignored[2] &&
                px[3] != vars->ignored[3])
            {
                printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x + 1, y + 1, px[0], px[1], px[2], px[3]);
                px[0] = 0;
                px[1] = 255;
                px[2] = 0;
                // Do something awesome for each pixel here...
            }
        }
    }
}

void modify_image_name(image_png *png)
{
    char name[] = "_modified";
    int new_length = strlen(png->name) + strlen(name) + 1;

    png->new_name = (char *)malloc(new_length);

    // strlength - .png length
    int file_name_length = strlen(png->name) - 4;

    for (int i = 0; i < file_name_length; i++)
    {
        png->new_name[i] = png->name[i];
    }

    for (int i = 0; i < strlen(name); i++)
    {
        png->new_name[i + file_name_length] = name[i];
    }

    png->new_name[new_length - 5] = '.';
    png->new_name[new_length - 4] = 'p';
    png->new_name[new_length - 3] = 'n';
    png->new_name[new_length - 2] = 'g';
    png->new_name[new_length - 1] = '\0';

    return;
}

void free_png(image_png *png)
{
    free(png->name);
    free(png->new_name);
    free(png->rows);
    free(png);
}

image_png *init_png_image(char *name)
{
    image_png *png = (image_png *)malloc(sizeof(image_png));
    png->name = name;
    png->new_name = NULL;
    png->height = 0;
    png->width = 0;
    png->color_type = 0;
    png->bit_depth = 0;
    png->rows = NULL;
    modify_image_name(png);
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
    // int width, height;
    // png_byte color_type, bit_depth;
    // png_bytep *rows = NULL;

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

    if (png_image->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        printf("YAY THIS HAS ALPHA");

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

// All below this line are deprecated

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