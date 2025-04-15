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

void cl_max_sort(int* arr, int size, int group) 
{
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem d_input, d_partial_max;
    cl_int err;

    char* kernelSource = readKernelSource("kernel/max.cl");

    err = clGetPlatformIDs(1, &platform, NULL);
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);

    cl_queue_properties properties[] = {CL_QUEUE_PROPERTIES, 0, 0};
    queue = clCreateCommandQueueWithProperties(context, device, properties, &err);

    program = clCreateProgramWithSource(context, 1, (const char**)&kernelSource, NULL, &err);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    kernel = clCreateKernel(program, "max_step_kernel", &err);

    for (int cursor = 0; cursor < size; cursor++)
    {
        int remaining = size - cursor;
        size_t globalSize = (remaining + group - 1) / group;

        d_input = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * remaining, arr + cursor, &err);
        d_partial_max = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * globalSize, NULL, &err);
        clFinish(queue);

        err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_input);
        err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_partial_max);
        err = clSetKernelArg(kernel, 2, sizeof(int), &group);
        err = clSetKernelArg(kernel, 3, sizeof(int), &remaining);

        err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);
        clFinish(queue);

        int* partial_max = malloc(sizeof(int) * globalSize);
        clEnqueueReadBuffer(queue, d_partial_max, CL_TRUE, 0, sizeof(int) * globalSize, partial_max, 0, NULL, NULL);

        int final_max = partial_max[0];
        for (int i = 1; i < globalSize; i++) if (partial_max[i] > final_max) final_max = partial_max[i];

        int max_index = cursor;
        for (int i = cursor; i < size; i++)
        {
            if (arr[i] == final_max)
            {
                max_index = i;
                break;
            }
        }

        int temp = arr[cursor];
        arr[cursor] = arr[max_index];
        arr[max_index] = temp;

        free(partial_max);
        clReleaseMemObject(d_input);
        clReleaseMemObject(d_partial_max);
    }

    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free(kernelSource);
}

void cl_quick_sort(int* arr, int size) 
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

    cl_mem buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, size * sizeof(int), arr, &err);
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
            int pivot;
            err = clEnqueueReadBuffer(queue, buffer, CL_TRUE, sizeof(int) * high, sizeof(int), &pivot, 0, NULL, NULL);

            err = clSetKernelArg(partition_kernel, 0, sizeof(cl_mem), &buffer);
            err |= clSetKernelArg(partition_kernel, 1, sizeof(int), &low);
            err |= clSetKernelArg(partition_kernel, 2, sizeof(int), &high);
            err |= clSetKernelArg(partition_kernel, 3, sizeof(cl_mem), &pivot_buffer);
            err |= clSetKernelArg(partition_kernel, 4, sizeof(int), &pivot);

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
        if (high - low + 1 > 1) 
        {
            err = clSetKernelArg(insertion_sort_kernel, 0, sizeof(cl_mem), &buffer);
            err |= clSetKernelArg(insertion_sort_kernel, 1, sizeof(int), &low);
            err |= clSetKernelArg(insertion_sort_kernel, 2, sizeof(int), &high);
            size_t global_size = 1;
            err = clEnqueueNDRangeKernel(queue, insertion_sort_kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);
            clFinish(queue);
        }
    }
    err = clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, size * sizeof(int), arr, 0, NULL, NULL);

    clReleaseMemObject(buffer);
    clReleaseMemObject(pivot_buffer);
    clReleaseKernel(partition_kernel);
    clReleaseKernel(insertion_sort_kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free((void*)kernelSource);
}

/*void cl_radix_sort(int* arr, int size, int max) 
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

    buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, size * sizeof(int), arr, &err);
    pivot_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &err);

    void quicksort_by_digit(int low, int high, int exp) 
    {
        if (low < high) 
        {
            int pivot;
            size_t offset = high * sizeof(int);
            err = clEnqueueReadBuffer(queue, buffer, CL_TRUE, offset, sizeof(int), &pivot, 0, NULL, NULL);
            int pivot_digit = (pivot / exp) % 10;
            err = clSetKernelArg(partition_kernel, 0, sizeof(cl_mem), &buffer);
            err |= clSetKernelArg(partition_kernel, 1, sizeof(int), &low);
            err |= clSetKernelArg(partition_kernel, 2, sizeof(int), &high);
            err |= clSetKernelArg(partition_kernel, 3, sizeof(cl_mem), &pivot_buffer);
            err |= clSetKernelArg(partition_kernel, 4, sizeof(int), &pivot_digit);

            size_t global_size = 1;
            err = clEnqueueNDRangeKernel(queue, partition_kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);
            clFinish(queue);

            int pivot_index;
            err = clEnqueueReadBuffer(queue, pivot_buffer, CL_TRUE, 0, sizeof(int), &pivot_index, 0, NULL, NULL);

            quicksort_by_digit(low, pivot_index - 1, exp);
            quicksort_by_digit(pivot_index + 1, high, exp);
        }
    }

    for (int exp = 1; max/exp > 0; exp *= 10) 
    {
        quicksort_by_digit(0, size - 1, exp);
    }

    err = clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, size * sizeof(int), arr, 0, NULL, NULL);

    clReleaseMemObject(buffer);
    clReleaseMemObject(pivot_buffer);
    clReleaseKernel(partition_kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free((void*)kernelSource);
}*/