#ifndef OPENCL_H
#define OPENCL_H

char* readKernelSource(const char* filename);

void cl_max_sort(unsigned long* arr, int size, int group);

void cl_quick_sort(unsigned long* arr, int size);

void cl_radix_sort(unsigned long* arr, int size, unsigned long max);

#endif