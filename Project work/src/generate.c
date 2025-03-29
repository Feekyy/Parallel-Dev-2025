#include "generate.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

unsigned long* generateRandomArray(int size, unsigned long min, unsigned long max) 
{
    unsigned long* arr = (unsigned long*)malloc(size * sizeof(unsigned long));
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

void writeArrayToFile(unsigned long* arr, int size, const char* filename)
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
        fprintf(file, "%lu\n", arr[i]);
    }

    fclose(file);
    free(fullPath);
}