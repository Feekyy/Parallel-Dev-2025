#ifndef GENERATE_H
#define GENERATE_H

unsigned long* generateRandomArray(int size, unsigned long min, unsigned long max);

void writeArrayToFile(unsigned long* arr, int size, const char* filename);

#endif