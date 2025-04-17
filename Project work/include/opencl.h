#ifndef OPENCL_H
#define OPENCL_H

#include <CL/cl.h>

void cl_radix_sort(int* arr, int size, int max);

char* readKernelSource(const char* filename);

void cl_max_sort(int* arr, int size, int group);

void cl_quick_sort(int* arr, int size);

void cl_radix_sort(int* arr, int size, int max);

void quicksort_by_digit(cl_context context, cl_command_queue queue, cl_program program, cl_mem buffer, cl_mem pivot_buffer, cl_kernel partition_kernel, int low, int high, int exp);

void quicksort_recursive(cl_context context, cl_command_queue queue, cl_program program, cl_mem buffer, cl_mem pivot_buffer, cl_kernel partition_kernel, int low, int high, int exp)

#endif