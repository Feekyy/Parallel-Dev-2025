#include "generate.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

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

void createSavesFolder() 
{
    #ifdef _WIN32
    if (mkdir("saves") == -1) 
    {
    #else
    if (mkdir("saves", 0777) == -1) 
    {
    #endif
        if (errno != EEXIST) 
        {
            fprintf(stderr, "Error creating 'saves' directory: %s\n", strerror(errno));
        }
    } 
    else 
    {
        printf("'saves' directory created successfully.\n");
    }
}

void deleteSavesFolder() 
{
    #ifdef _WIN32
    if (system("rmdir /s /q saves") != 0) 
    {
    #else
    if (system("rm -rf saves") != 0) 
    {
    #endif
        fprintf(stderr, "Error deleting 'saves' directory.\n");
    } 
    else 
    {
        printf("'saves' directory deleted successfully.\n");
    }
}