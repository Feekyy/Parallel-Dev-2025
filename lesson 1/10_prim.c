#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>
#include <math.h>

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
    int n;
    printf("Enter a number to test for primality: ");
    scanf("%d", &n);

    char *source_str = kernelInput("10_kern1.cl");
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

    int num_work_items = 256;
    int range = (int)ceil((double)n / (2 * num_work_items));
    int start_divisor = 2;

    int *flag = (int*)malloc(num_work_items * sizeof(int));
    memset(flag, 0, num_work_items * sizeof(int));

    cl_mem flag_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, num_work_items * sizeof(int), NULL, &ret);
    
    ret = clEnqueueWriteBuffer(command_queue, flag_mem_obj, CL_TRUE, 0, num_work_items * sizeof(int), flag, 0, NULL, NULL);
    
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    
    cl_kernel kernel = clCreateKernel(program, "prime_test_range", &ret);
    
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&flag_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(int), &n);
    ret = clSetKernelArg(kernel, 2, sizeof(int), &start_divisor);
    ret = clSetKernelArg(kernel, 3, sizeof(int), &range);
    
    size_t global_item_size = num_work_items;
    size_t local_item_size = 64;
    
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
    
    ret = clEnqueueReadBuffer(command_queue, flag_mem_obj, CL_TRUE, 0, num_work_items * sizeof(int), flag, 0, NULL, NULL);
    
    int is_prime = 1;
    for (int i = 0; i < num_work_items; i++) 
    {
        if (flag[i] == 1) 
        {
            is_prime = 0;
            break;
        }
    }
    
    if (is_prime && n > 1)
        printf("%d is prime.\n", n);
    else
        printf("%d is not prime.\n", n);
    
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(flag_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    
    free(source_str);
    free(flag);
    return 0;
}