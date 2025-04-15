#ifndef OPENCL_H
#define OPENCL_H

char* readKernelSource(const char* filename);

void cl_max_sort(int* arr, int size, int group);

void cl_quick_sort(int* arr, int size);

void cl_radix_sort(int* arr, int size, int max);

#endif