#ifndef GENERATE_H
#define GENERATE_H

int* generateRandomArray(int size, int min, int max);

void writeArrayToFile(int* arr, int size, const char* filename);

#endif