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
        printf("[ERROR] Nem sikerült megnyitni a kernel fájlt.\n");
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
    int input[10] = {5, 2, 8, 10, 9, 1, 7, 1, -1, 0};
    int output[2] = {INT_MAX, INT_MIN};
    
    char *source_str = kernelInput("08_kern_opt.cl");
    if(source_str == NULL) return 1;
    size_t source_size = strlen(source_str);

    cl_int ret;
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    if(ret != CL_SUCCESS)
    {
        printf("Hiba: clGetPlatformIDs sikertelen (%d).\n", ret);
        return 1;
    }
    
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
    if(ret != CL_SUCCESS)
    {
        printf("Hiba: clGetDeviceIDs sikertelen (%d).\n", ret);
        return 1;
    }
    
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    if(ret != CL_SUCCESS)
    {
        printf("Hiba: clCreateContext sikertelen (%d).\n", ret);
        return 1;
    }
    
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
    if(ret != CL_SUCCESS)
    {
        printf("Hiba: clCreateCommandQueue sikertelen (%d).\n", ret);
        return 1;
    }
    
    cl_mem input_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, n * sizeof(int), NULL, &ret);
    cl_mem output_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, 2 * sizeof(int), NULL, &ret);
    
    ret = clEnqueueWriteBuffer(command_queue, output_mem_obj, CL_TRUE, 0, 2 * sizeof(int), output, 0, NULL, NULL);
    if(ret != CL_SUCCESS)
    {
        printf("Hiba: clEnqueueWriteBuffer sikertelen (%d).\n", ret);
        return 1;
    }
    
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
    if(ret != CL_SUCCESS)
    {
        printf("Hiba: clCreateProgramWithSource sikertelen (%d).\n", ret);
        return 1;
    }
    
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    if(ret != CL_SUCCESS)
    {
        size_t log_size;
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char *log = (char*) malloc(log_size);
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        printf("Hiba a fordítás során:\n%s\n", log);
        free(log);
        return 1;
    }
    
    cl_kernel kernel = clCreateKernel(program, "find_minmax", &ret);
    if(ret != CL_SUCCESS)
    {
        printf("Hiba: clCreateKernel sikertelen (%d).\n", ret);
        return 1;
    }
    
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&input_mem_obj);
    ret |= clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&output_mem_obj);
    ret |= clSetKernelArg(kernel, 2, sizeof(int), &n);
    if(ret != CL_SUCCESS)
    {
        printf("Hiba: clSetKernelArg sikertelen (%d).\n", ret);
        return 1;
    }
    
    size_t global_item_size = 256;
    size_t local_item_size = 64;

    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
    if(ret != CL_SUCCESS)
    {
        printf("Hiba: clEnqueueNDRangeKernel sikertelen (%d).\n", ret);
        return 1;
    }
    
    ret = clEnqueueReadBuffer(command_queue, output_mem_obj, CL_TRUE, 0, n * sizeof(int), output, 0, NULL, NULL);
    if(ret != CL_SUCCESS)
    {
        printf("Hiba: clEnqueueReadBuffer sikertelen (%d).\n", ret);
        return 1;
    }
    
    printf("Input array: ");
    for (int i = 0; i < n; i++) 
    {
        printf("%d ", input[i]);
    }
    printf("\n");
    
    printf("Minimum value: %d\n", output[0]);
    printf("Maximum value: %d\n", output[1]);
    
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
