#include "generate.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#ifdef _WIN32
#include <direct.h>
#endif

#define FIXED_SEED 12345

int* generateRandomArray(int size, int min, int max) 
{
    int* arr = (int*)malloc(size * sizeof(int));
    if (arr == NULL) 
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    srand(FIXED_SEED);

    for (int i = 0; i < size; i++) 
    {
        arr[i] = (rand() % (max - min + 1)) + min;
    }

    return arr;
}

void writeArrayToFile(int* arr, int size, const char* filename)
{
    char fullPath[512];
    snprintf(fullPath, sizeof(fullPath), "saves/%s", filename);

    FILE* file = fopen(fullPath, "w");
    if (file == NULL) 
    {
        fprintf(stderr, "Failed to open file: %s\n", fullPath);
        return;
    }

    for (int i = 0; i < size; i++) 
    {
        fprintf(file, "%d\n", arr[i]);
    }

    fclose(file);
}

void createSavesFolder() 
{
    #ifdef _WIN32
    if (_mkdir("saves") == -1)
    #else
    if (mkdir("saves", 0777) == -1)
    #endif
    {
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
    #else
    if (system("rm -rf saves") != 0) 
    #endif
    {
        fprintf(stderr, "Error deleting 'saves' directory.\n");
    } 
    else 
    {
        printf("'saves' directory deleted successfully.\n");
    }
}

double runAlgorithm(test_data* data)
{
    int* arr = generateRandomArray(data->size, data->min, data->max);
    if (arr == NULL) 
    {
        fprintf(stderr, "Memory allocation failed in generateRandomArray\n");
        return -1;
    }
    
    char filename[256];
    snprintf(filename, sizeof(filename), "DEFF_%s", data->save_filename);
    writeArrayToFile(arr, data->size, filename);

    clock_t start = clock();
    
    data->algorithm(arr, data->size, data->max);

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    snprintf(filename, sizeof(filename), "SORT_%s", data->save_filename);
    writeArrayToFile(arr, data->size, filename);

    free(arr);

    return cpu_time_used;
}