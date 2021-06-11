#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "image.h"
#include "../linked_list/string_linked_list.h"
#include "../main.h"

char *modify_image_name(char *file_name)
{
    char name[] = "_modified";
    int new_length = strlen(file_name) + strlen(name) + 1;

    char *new_name = (char *)malloc(new_length);

    // strlength - .png length
    int file_name_length = strlen(file_name) - 4;

    for (int i = 0; i < file_name_length; i++)
    {
        new_name[i] = file_name[i];
    }

    for (int i = 0; i < strlen(name); i++)
    {
        new_name[i + file_name_length] = name[i];
    }

    new_name[new_length - 5] = '.';
    new_name[new_length - 4] = 'p';
    new_name[new_length - 3] = 'n';
    new_name[new_length - 2] = 'g';
    new_name[new_length - 1] = '\0';

    return new_name;
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