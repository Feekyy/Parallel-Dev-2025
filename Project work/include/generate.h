#ifndef GENERATE_H
#define GENERATE_H

// Function to generate a random array of given size and range
int* generateRandomArray(int size, int min, int max);

// Function to write the generated array to a file
void writeArrayToFile(int* arr, int size, const char* filename);

#endif // GENERATE_H