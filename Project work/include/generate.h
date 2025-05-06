#ifndef GENERATE_H
#define GENERATE_H

typedef void (*AlgorithmFunction)(int*, int, int);

typedef struct test_data
{
    int size;
    int min;
    int max;
    int* arr;
    AlgorithmFunction algorithm;
    char* save_filename;
} test_data;

// Function to generate a random array of given size and range
int* generateRandomArray(int size, int min, int max);

// Function to write the generated array to a file
void writeArrayToFile(int* arr, int size, const char* filename);

// Function to create the saves folder if it doesn't exist
void createSavesFolder();

// Function to delete the saves folder if it exists
void deleteSavesFolder();

// Function to run  algorithm
double runAlgorithm(test_data* data);

#endif // GENERATE_H