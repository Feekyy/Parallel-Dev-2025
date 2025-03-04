#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

char* kernelInput(const char* filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file) 
    {
        printf("[ERROR] Failed to open the kernel file.\n");
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);
    
    char* code = (char*) malloc(file_size + 1);
    fread(code, sizeof(char), file_size, file);
    code[file_size] = '\0';
    fclose(file);
    
    return code;
}

int main()
{
    int n = 10;
    int radius = 2;
    float input[10] = {10.0f, 22.0f, 3.0f, 9.0f, 14.0f, 21.0f, 7.0f, 0.0f, 9.0f, 18.0f};
    float output[10] = {0.0f};
    
    char *source_str = kernelInput("09_kern.cl");
    if(source_str == NULL) return 1;
    size_t source_size = strlen(source_str);

    cl_int ret;
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
    
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
    
    cl_mem input_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, n * sizeof(float), NULL, &ret);
    cl_mem output_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, n * sizeof(float), NULL, &ret);
    
    ret = clEnqueueWriteBuffer(command_queue, input_mem_obj, CL_TRUE, 0, n * sizeof(float), input, 0, NULL, NULL);
    
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    
    cl_kernel kernel = clCreateKernel(program, "moving_average", &ret);
    
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&input_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&output_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(int), &n);
    ret = clSetKernelArg(kernel, 3, sizeof(int), &radius);
    
    size_t global_item_size = n;
    size_t local_item_size = 1;
    
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
    
    ret = clEnqueueReadBuffer(command_queue, output_mem_obj, CL_TRUE, 0, n * sizeof(float), output, 0, NULL, NULL);
    
    printf("Input array: ");
    for (int i = 0; i < n; i++) 
    {
        printf("%.2f ", input[i]);
    }
    printf("\n");
    
    printf("Moving average result: ");
    for (int i = 0; i < n; i++) 
    {
        printf("%.2f ", output[i]);
    }
    printf("\n");
    
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(input_mem_obj);
    ret = clReleaseMemObject(output_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    
    free(source_str);
    return 0;
}