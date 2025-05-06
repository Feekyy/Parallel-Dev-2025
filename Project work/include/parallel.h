#ifndef PARA_H
#define PARA_H

#include <CL/cl.h>

// Function to read the kernel source code from a file
char* readKernelSource(const char* filename);

// Function to perform radix sort on the array
void cl_max_sort(int* arr, int size, int group);

// Function to start the quicksort on the array
void cl_quick_sort(int* arr, int size);

// The recursive part to perform quicksort on the array
void quicksort_recursive(cl_context context, cl_command_queue queue, cl_program program, cl_mem buffer, cl_mem pivot_buffer, cl_kernel partition_kernel, int low, int high);

// Function to start the radixsort on the array
void cl_radix_sort(int* arr, int size, int max);

// The partitioning part to perform radixsort on the array
void quicksort_by_digit(cl_context context, cl_command_queue queue, cl_program program, cl_mem buffer, cl_mem pivot_buffer, cl_kernel partition_kernel, int low, int high, int exp);

// Wrapper function
void cl_max_sort_wrapper(int* arr, int size, int max);

// Wrapper function
void cl_quick_sort_wrapper(int* arr, int size, int max);

// Wrapper function
void cl_radix_sort_wrapper(int* arr, int size, int max);

#endif // PARA_H