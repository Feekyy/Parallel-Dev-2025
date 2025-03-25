#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>

typedef struct Image
{
    FILE *fp;
    int height;
    int width;
    int maxCol;
    int* picMatrix[width][height];
} Image;

void import(Image* ima, char filename[20]);

#endif