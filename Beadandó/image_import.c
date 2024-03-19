#include <stdio.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "image_import.h"


void import(Image* ima, char filename[20])
{
    int height;
    int width;

    unsigned char* rgb_data;
    char path[] = "images/";
    strcat(path, filename);
    printf("\nPath: %s\n", path);
    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        printf("File is empty!");
        return;
    }

    int channels;
    rgb_data = stbi_load_from_file(file, &width, &height, &channels, 3);

    fclose(file);

    FILE *out = fopen("images/colors_horse.ppm", "wb");

    fprintf(out, "P3\n");
    fprintf(out, "%d %d\n", width, height);
    fprintf(out, "255\n");

    for (int i = 0; i < height*width; ++i) 
    {
            fprintf(out, "%d %d %d ", rgb_data[3 * i], rgb_data[3 * i + 1], rgb_data[3 * i + 2]);
    }

    ima->fp = out;
    ima->height = height;
    ima->width = width;
    fclose(out);
    stbi_image_free(rgb_data);

    printf("\n\nPicture successfuly imported!\n");
}