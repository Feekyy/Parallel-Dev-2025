#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


int main() 
{
    int height;
    int width;

    unsigned char* rgb_data;
    FILE *file = fopen("images/horse.png", "rb");

    if (file == NULL)
    {
        printf("File is empty!");
        return 0;
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

    fclose(out);

    stbi_image_free(rgb_data);
}