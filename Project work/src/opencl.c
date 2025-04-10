# include "opencl.h"

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>


char* readKernelSource(const char* filename) 
{
    FILE* file = fopen(filename, "r");
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
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem d_input, d_partial_max;

    unsigned long* partial_max = (unsigned long*)malloc(sizeof(unsigned long) * (size / group));

    char* kernelSource = readKernelSource("kernel/max.cl");

    cl_int err;
    err = clGetPlatformIDs(1, &platform, NULL);

    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);

    queue = clCreateCommandQueue(context, device, 0, &err);

    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err);

    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    kernel = clCreateKernel(program, "max_step_kernel", &err);

    d_input = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(unsigned long) * size, &arr, &err);

    d_partial_max = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(unsigned long) * (size / group), NULL, &err);

    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_input);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_partial_max);
    err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &group);
    err |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &size);

    size_t globalSize = size / group;
    size_t localSize = 64;

    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL);

    err = clEnqueueReadBuffer(queue, d_partial_max, CL_TRUE, 0, sizeof(unsigned long) * (size / group), partial_max, 0, NULL, NULL);

    unsigned long final_max = partial_max[0];
    for (int i = 1; i < size / group; i++) 
    {
        if (partial_max[i] > final_max) 
        {
            final_max = partial_max[i];
        }
    }

    clReleaseMemObject(d_input);
    clReleaseMemObject(d_partial_max);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

}

void cl_quick_sort(unsigned long* arr, int size) 
{
    cl_int err;
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;

    err = clGetPlatformIDs(1, &platform, NULL);
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    queue = clCreateCommandQueue(context, device, 0, &err);

    const char* kernelSource = readKernelSource("kernel/quick.cl");
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    kernel = clCreateKernel(program, "quicksort_iterative", &err);

    cl_mem buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, size * sizeof(unsigned long), arr, &err);

    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer);
    err = clSetKernelArg(kernel, 1, sizeof(int), &size);

    size_t global_size = 1;
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);
    clFinish(queue);

    err = clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, size * sizeof(unsigned long), arr, 0, NULL, NULL);

    clReleaseMemObject(buffer);
    clReleaseKernel(kernel);
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
    cl_kernel radix_sort_kernel;

    err = clGetPlatformIDs(1, &platform, NULL);
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    queue = clCreateCommandQueue(context, device, 0, &err);

    const char* kernelSource = readKernelSource("kernel/radix.cl");
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    radix_sort_kernel = clCreateKernel(program, "radix_sort_kernel", &err);

    cl_mem input_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, size * sizeof(unsigned long), arr, &err);
    cl_mem output_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, size * sizeof(unsigned long), NULL, &err);
    cl_mem count_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, 16 * sizeof(int), NULL, &err);

    unsigned long* output = (unsigned long*)malloc(size * sizeof(unsigned long));
    for (unsigned int shift = 0; shift < 64; shift += 4) {
        int count[16] = {0};

        err = clSetKernelArg(radix_sort_kernel, 0, sizeof(cl_mem), &input_buffer);
        err = clSetKernelArg(radix_sort_kernel, 1, sizeof(cl_mem), &output_buffer);
        err = clSetKernelArg(radix_sort_kernel, 2, sizeof(cl_mem), &count_buffer);
        err = clSetKernelArg(radix_sort_kernel, 3, sizeof(unsigned int), &shift);
        err = clSetKernelArg(radix_sort_kernel, 4, sizeof(unsigned int), &size);

        size_t global_size = size;
        err = clEnqueueNDRangeKernel(queue, radix_sort_kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);

        err = clEnqueueReadBuffer(queue, count_buffer, CL_TRUE, 0, 16 * sizeof(int), count, 0, NULL, NULL);

        for (int i = 1; i < 16; i++) 
        {
            count[i] += count[i-1];
        }

        for (int i = size - 1; i >= 0; i--) 
        {
            unsigned int digit = (arr[i] >> shift) & 0xF;
            output[--count[digit]] = arr[i];
        }

        unsigned long* temp = arr;
        arr = output;
        output = temp;

        err = clEnqueueWriteBuffer(queue, input_buffer, CL_TRUE, 0, size * sizeof(unsigned long), arr, 0, NULL, NULL);
    }

    err = clEnqueueReadBuffer(queue, input_buffer, CL_TRUE, 0, size * sizeof(unsigned long), arr, 0, NULL, NULL);

    free(output);
    clReleaseMemObject(input_buffer);
    clReleaseMemObject(output_buffer);
    clReleaseMemObject(count_buffer);
    clReleaseKernel(radix_sort_kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free((void*)kernelSource);
}