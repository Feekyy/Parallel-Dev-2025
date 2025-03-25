#ifndef IMAGE_H
#define IMAGE_H

typedef struct Image
{
    FILE *fp;
    int height;
    int width;
    int maxCol;
    int** picMatrix;
} Image;

void import(Image* ima, char filename[20]);

#endif