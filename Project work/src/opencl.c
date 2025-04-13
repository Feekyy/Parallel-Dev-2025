#include "opencl.h"
#include "sorting.h"

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define INSERTION_SORT_THRESHOLD 1024

char* readKernelSource(const char* filename) 
{
    FILE* file = fopen(filename, "rb");
    if (!file) 
    {
        printf("Failed to open kernel file\n");
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* source = (char*)malloc(length + 1);
    fread(source, 1, length, file);
    source[length] = '\0';
    fclose(file);
    return source;
}

void cl_max_sort(unsigned long* arr, int size, int group) 
{
    for (int i = 0; i < size; i++) 
    {
        printf("input[%d] = %lu\n", i, arr[i]);
    }

    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem d_input, d_partial_max;
    cl_int err;

    size_t globalSize = (size + group - 1) / group;

    unsigned long* partial_max = (unsigned long*)malloc(sizeof(unsigned long) * globalSize);

    char* kernelSource = readKernelSource("kernel/max.cl");

    err = clGetPlatformIDs(1, &platform, NULL);
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);

    cl_queue_properties properties[] = {CL_QUEUE_PROPERTIES, 0, 0};
    queue = clCreateCommandQueueWithProperties(context, device, properties, &err);

    program = clCreateProgramWithSource(context, 1, (const char**)&kernelSource, NULL, &err);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    kernel = clCreateKernel(program, "max_step_kernel", &err);

    cl_ulong* input = arr;

    d_input = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_ulong) * size, input, &err);
    d_partial_max = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_ulong) * globalSize, NULL, &err);

    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_input);
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_partial_max);
    err = clSetKernelArg(kernel, 2, sizeof(int), &group);
    err = clSetKernelArg(kernel, 3, sizeof(int), &size);

    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);
    clFinish(queue);

    err = clEnqueueReadBuffer(queue, d_partial_max, CL_TRUE, 0, sizeof(cl_ulong) * globalSize, partial_max, 0, NULL, NULL);

    printf("Partial maximums:\n");
    for (int i = 0; i < size / group; i++) 
    {
        printf("%lu ", partial_max[i]);
    }
    printf("\n");

    unsigned long final_max = partial_max[0];
    for (int i = 1; i < size / group; i++) 
    {
        if (partial_max[i] > final_max) 
        {
            final_max = partial_max[i];
        }
    }
    printf("Final max: %lu\n", final_max);

    for (int i = 0; i < size; i++) 
    {
        arr[i] = final_max;
    }

    clReleaseMemObject(d_input);
    clReleaseMemObject(d_partial_max);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free(kernelSource);
    free(partial_max);
}

void cl_quick_sort(unsigned long* arr, int size) 
{
    cl_int err;
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel partition_kernel, insertion_sort_kernel;

    err = clGetPlatformIDs(1, &platform, NULL);
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    cl_queue_properties properties[] = {CL_QUEUE_PROPERTIES, 0, 0};
    queue = clCreateCommandQueueWithProperties(context, device, properties, &err);

    const char* kernelSource = readKernelSource("kernel/quick.cl");
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    partition_kernel = clCreateKernel(program, "partition", &err);
    insertion_sort_kernel = clCreateKernel(program, "insertion_sort", &err);

    cl_mem buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, size * sizeof(unsigned long), arr, &err);
    cl_mem pivot_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &err);

    int stack[2 * (sizeof(int) * 8)];
    int top = -1;
    stack[++top] = 0;
    stack[++top] = size - 1;

    while (top >= 0) 
    {
        int high = stack[top--];
        int low = stack[top--];

        while (high - low > INSERTION_SORT_THRESHOLD) 
        {
            unsigned long pivot;
            err = clEnqueueReadBuffer(queue, buffer, CL_TRUE, sizeof(unsigned long) * high, sizeof(unsigned long), &pivot, 0, NULL, NULL);

            err = clSetKernelArg(partition_kernel, 0, sizeof(cl_mem), &buffer);
            err |= clSetKernelArg(partition_kernel, 1, sizeof(int), &low);
            err |= clSetKernelArg(partition_kernel, 2, sizeof(int), &high);
            err |= clSetKernelArg(partition_kernel, 3, sizeof(cl_mem), &pivot_buffer);
            err |= clSetKernelArg(partition_kernel, 4, sizeof(unsigned long), &pivot);

            size_t global_size = high - low + 1;
            err = clEnqueueNDRangeKernel(queue, partition_kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);
            clFinish(queue);

            int pivot_index;
            err = clEnqueueReadBuffer(queue, pivot_buffer, CL_TRUE, 0, sizeof(int), &pivot_index, 0, NULL, NULL);

            if (pivot_index > low && pivot_index < high) 
            {
                if (pivot_index - low < high - pivot_index) 
                {
                    stack[++top] = pivot_index + 1;
                    stack[++top] = high;
                    high = pivot_index - 1;
                } 
                else 
                {
                    stack[++top] = low;
                    stack[++top] = pivot_index - 1;
                    low = pivot_index + 1;
                }
            } 
            else 
            {
                int mid = low + (high - low) / 2;
                stack[++top] = mid + 1;
                stack[++top] = high;
                high = mid;
            }
        }
        if (high - low + 1 > 1) {
            err = clSetKernelArg(insertion_sort_kernel, 0, sizeof(cl_mem), &buffer);
            err |= clSetKernelArg(insertion_sort_kernel, 1, sizeof(int), &low);
            err |= clSetKernelArg(insertion_sort_kernel, 2, sizeof(int), &high);
            size_t global_size = 1;
            err = clEnqueueNDRangeKernel(queue, insertion_sort_kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);
            clFinish(queue);
        }
    }
    err = clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, size * sizeof(unsigned long), arr, 0, NULL, NULL);

    clReleaseMemObject(buffer);
    clReleaseMemObject(pivot_buffer);
    clReleaseKernel(partition_kernel);
    clReleaseKernel(insertion_sort_kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free((void*)kernelSource);
}

void cl_radix_sort(unsigned long* arr, int size, unsigned long max) 
{
    cl_int err;
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel partition_kernel;
    cl_mem buffer = NULL, pivot_buffer = NULL;

    err = clGetPlatformIDs(1, &platform, NULL);
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    cl_queue_properties properties[] = {CL_QUEUE_PROPERTIES, 0, 0};
    queue = clCreateCommandQueueWithProperties(context, device, properties, &err);

    const char* kernelSource = readKernelSource("kernel/quick.cl");
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    partition_kernel = clCreateKernel(program, "partition", &err);

    buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, size * sizeof(unsigned long), arr, &err);
    pivot_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &err);

    void quicksort_by_digit(int low, int high, unsigned long exp) 
    {
        if (low < high) 
        {
            unsigned long pivot;
            size_t offset = high * sizeof(unsigned long);
            err = clEnqueueReadBuffer(queue, buffer, CL_TRUE, offset, sizeof(unsigned long), &pivot, 0, NULL, NULL);
            unsigned long pivot_digit = (pivot / exp) % 10;
            err = clSetKernelArg(partition_kernel, 0, sizeof(cl_mem), &buffer);
            err |= clSetKernelArg(partition_kernel, 1, sizeof(int), &low);
            err |= clSetKernelArg(partition_kernel, 2, sizeof(int), &high);
            err |= clSetKernelArg(partition_kernel, 3, sizeof(cl_mem), &pivot_buffer);
            err |= clSetKernelArg(partition_kernel, 4, sizeof(unsigned long), &pivot_digit);

            size_t global_size = 1;
            err = clEnqueueNDRangeKernel(queue, partition_kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);
            clFinish(queue);

            int pivot_index;
            err = clEnqueueReadBuffer(queue, pivot_buffer, CL_TRUE, 0, sizeof(int), &pivot_index, 0, NULL, NULL);

            quicksort_by_digit(low, pivot_index - 1, exp);
            quicksort_by_digit(pivot_index + 1, high, exp);
        }
    }

    for (unsigned long exp = 1; max/exp > 0; exp *= 10) 
    {
        quicksort_by_digit(0, size - 1, exp);
    }

    err = clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, size * sizeof(unsigned long), arr, 0, NULL, NULL);

    clReleaseMemObject(buffer);
    clReleaseMemObject(pivot_buffer);
    clReleaseKernel(partition_kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free((void*)kernelSource);
}