#include <stdio.h>
#include <stdlib.h>

#include "image_import.h"
#include "countor.h"

void edgeDetection(Image* ima)
{
    FILE *fp = fopen("images/countur.pgm", "wb+");

    int hmatrix[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int vmatrix[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int threshold = 125;

    int** matrix = ima->picMatrix;

    for (int i = 0; i < ima->height; i++)
    {
        for (int k = 0; k < ima->width; k++)
        {
            printf("%d ", ima->picMatrix[i][k]);
        }
        printf("\n");
    }

    fprintf(fp, "P2\n");
    fprintf(fp, "%d %d\n", ima->width, ima->height);
    fprintf(fp, "255\n");

    for (int y = 0; y < ima->height; y++) 
    {
        for (int x = 0; x < ima->width; x++) 
        {
            if (y == 0 || y == ima->height-1 || x == 0 || x == ima->width-1)
            {
                fprintf(fp, "%d ", 255);
                continue;
            }


            int horizontalGradient = 0;
            int verticalGradient = 0;

            for (int i = -1; i <= 1; i++) 
            {
                for (int j = -1; j <= 1; j++) 
                {
                    horizontalGradient += matrix[y + i][x + j] * hmatrix[i + 1][j + 1];
                    verticalGradient += matrix[y + i][x + j] * vmatrix[i + 1][j + 1];
                }
            }

            int totalGradient = abs(horizontalGradient) + abs(verticalGradient);

            if (totalGradient > threshold) 
            {
                fprintf(fp, "%d ", 255);
            } 
            else 
            {
                fprintf(fp, "%d ", 0);
            }
            //printf("x: %d, y: %d, horizontal: %d, vertical: %d, total: %d\n", x, y, horizontalGradient, verticalGradient, totalGradient);
        }
        //fprintf(fp, "\n");
    }

    ima->fp = fp;
    fclose(fp);

    printf("Countur successfuly made!\n");
}

int** convertToIntMatrix(Image *ima)
{
    int **intMatrix = (int **)malloc(ima->height * sizeof(int *));
    for (int i = 0; i < ima->height; i++) 
    {
        intMatrix[i] = (int *)malloc(ima->width * sizeof(int));
        for (int j = 0; j < ima->width; j++) 
        {
            intMatrix[i][j] = (int)ima->picMatrix[i][j];
            printf("%d ", intMatrix[i][j]);
        }
        printf("\n");
    }
    return intMatrix;
}