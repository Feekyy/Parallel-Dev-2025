#include "generate.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int* generateRandomArray(int size, int min, int max) 
{
    int* arr = (int*)malloc(size * sizeof(int));
    if (arr == NULL) 
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    srand(time(NULL));

    for (int i = 0; i < size; i++) 
    {
        arr[i] = (rand() % (max - min + 1)) + min;
    }

    return arr;
}

void writeArrayToFile(int* arr, int size, const char* filename)
{
    char* fullPath = malloc(strlen("saves/") + strlen(filename) + 1);
    if (fullPath == NULL) 
    {
        fprintf(stderr, "Memory allocation failed for filename\n");
        return;
    }

    strcpy(fullPath, "saves/");
    strcat(fullPath, filename);

    FILE* file = fopen(fullPath, "w");
    if (file == NULL) 
    {
        fprintf(stderr, "Failed to open file: %s\n", fullPath);
        free(fullPath);
        return;
    }

    for (int i = 0; i < size; i++) 
    {
        fprintf(file, "%d\n", arr[i]);
    }

    fclose(file);
    free(fullPath);
}