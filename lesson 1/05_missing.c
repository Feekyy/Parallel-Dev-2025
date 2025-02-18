#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CL/cl.h>

#define SAMPLE_SIZE 10

char* kernelInput()
{
    FILE *file = fopen("04_kern.cl", "r");

    if (!file) 
    {
        printf("[ERROR] Could not open kernel file.\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* code = malloc(file_size + 1);

    fread(code, sizeof(char), file_size, file);
    code[file_size] = '\0';
    fclose(file);
    return code;
}

int main(void)
{
    int i;
    cl_int err;
    const char* kernel_code = kernelInput();

    cl_uint n_platforms;
	cl_platform_id platform_id;
    err = clGetPlatformIDs(1, &platform_id, &n_platforms);
	if (err != CL_SUCCESS) 
    {
		printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n", err);
		return 0;
	}

	cl_device_id device_id;
	cl_uint n_devices;
	err = clGetDeviceIDs(
		platform_id,
		CL_DEVICE_TYPE_GPU,
		1,
		&device_id,
		&n_devices
	);
	if (err != CL_SUCCESS) 
    {
		printf("[ERROR] Error calling clGetDeviceIDs. Error code: %d\n", err);
		return 0;
	}

    cl_context context = clCreateContext(NULL, n_devices, &device_id, NULL, NULL, NULL);

    cl_program program = clCreateProgramWithSource(context, 1, &kernel_code, NULL, NULL);
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS) 
    {
        printf("Build error! Code: %d\n", err);
        return 0;
    }
    cl_kernel kernel = clCreateKernel(program, "find_missing", NULL);

    int* buffer = (int*)malloc(SAMPLE_SIZE * sizeof(int));

    int buffer[SAMPLE_SIZE] = {10, 0, 34, 10, 23, 0, 7, 0, 1, 10};

    cl_mem device_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, SAMPLE_SIZE * sizeof(int), NULL, NULL);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer);
    clSetKernelArg(kernel, 3, sizeof(int), &SAMPLE_SIZE);

    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, NULL, NULL);

    clEnqueueWriteBuffer(command_queue, buffer, CL_FALSE, 0, SAMPLE_SIZE * sizeof(float), buffer, 0, NULL, NULL);

    size_t local_work_size = 256;
    size_t n_work_groups = (SAMPLE_SIZE + local_work_size + 1) / local_work_size;
    size_t global_work_size = n_work_groups * local_work_size;

    clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);

    clEnqueueReadBuffer(command_queue, buffer, CL_TRUE, 0, SAMPLE_SIZE * sizeof(float), buffer, 0, NULL, NULL);

    for (int i = 0; i < SAMPLE_SIZE; ++i) 
    {
        printf("[%d] = %.2f, \n", i, buffer[i]);
    }

    clReleaseMemObject(buffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseDevice(device_id);

    free(buffer);
}
