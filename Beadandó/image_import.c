#include <stdio.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "image_import.h"


void import(Image* ima, char filename[20])
{
    int height;
    int width;

    int* rgb_data;
    char path[] = "images/";
    strcat(path, filename);
    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        printf("File does not exist!\n");
        return;
    }

    int channels;
    rgb_data = stbi_load_from_file(file, &width, &height, &channels, 3);

    fclose(file);

    FILE *out = fopen("images/greymap.pgm", "wb+");

    fprintf(out, "P2\n");
    fprintf(out, "%d %d\n", width, height);
    fprintf(out, "255\n");

    int k = 3 * width;
    ima->picMatrix = (unsigned char**)malloc(height * sizeof(unsigned char*));

    for (int i = 0; i < height; ++i) 
    {
        ima->picMatrix[i] = (int *)malloc(width * sizeof(int));
        for (int k = 0; k < width; k++)
        {

            ima->picMatrix[i][k] = (rgb_data[(3 * i * width) + k] + rgb_data[(3 * i * width + 1) + k] + rgb_data[(3 * i * width + 2) + k])/3;
            printf("Matrixnumb: %d , i:%d, k:%d, 1:%d, 2:%d, 3:%d\n", ima->picMatrix[i][k], i, k, rgb_data[(3 * i * width) + k], rgb_data[(3 * i * width + 1)], rgb_data[(3 * i * width + 2) + k]);

        }
        printf("\n");
    }

    for (int i = 0; i < height*width; ++i)
    {
        fprintf(out, "%d ", (rgb_data[3 * i] + rgb_data[3 * i + 1] + rgb_data[3 * i + 2])/3);
    }

    ima->fp = out;
    ima->height = height;
    ima->width = width;
    ima->maxCol = 255;
    fclose(out);
    stbi_image_free(rgb_data);

    printf("\n\nPicture successfuly imported!\n");
}