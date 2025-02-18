#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CL/cl.h>

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

const int SAMPLE_SIZE = 10;

int main(void)
{
    int i;
    cl_int err;
    const char* kernel_code = kernelInput();

    cl_uint n_platforms;
	cl_platform_id platform_id;
    err = clGetPlatformIDs(1, &platform_id, &n_platforms);
	if (err != CL_SUCCESS) {
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
	if (err != CL_SUCCESS) {
		printf("[ERROR] Error calling clGetDeviceIDs. Error code: %d\n", err);
		return 0;
	}

    cl_context context = clCreateContext(NULL, n_devices, &device_id, NULL, NULL, NULL);

    cl_program program = clCreateProgramWithSource(context, 1, &kernel_code, NULL, NULL);
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Build error! Code: %d\n", err);
        return 0;
    }
    cl_kernel kernel = clCreateKernel(program, "vector_add", NULL);

    float* host_a = (float*)malloc(SAMPLE_SIZE * sizeof(float));
    float* host_b = (float*)malloc(SAMPLE_SIZE * sizeof(float));
    float* host_c = (float*)malloc(SAMPLE_SIZE * sizeof(float));
    for (i = 0; i < SAMPLE_SIZE; ++i) 
    {
        host_a[i] = i * 1.0f;
        host_b[i] = i * 2.0f;
    }

    cl_mem device_a = clCreateBuffer(context, CL_MEM_READ_ONLY, SAMPLE_SIZE * sizeof(float), NULL, NULL);
    cl_mem device_b = clCreateBuffer(context, CL_MEM_READ_ONLY, SAMPLE_SIZE * sizeof(float), NULL, NULL);
    cl_mem device_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, SAMPLE_SIZE * sizeof(float), NULL, NULL);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&device_a);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&device_b);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&device_c);
    clSetKernelArg(kernel, 3, sizeof(int), (void*)&SAMPLE_SIZE);

    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, NULL, NULL);

    clEnqueueWriteBuffer(command_queue, device_a, CL_FALSE, 0, SAMPLE_SIZE * sizeof(float), host_a, 0, NULL, NULL);
    clEnqueueWriteBuffer(command_queue, device_b, CL_FALSE, 0, SAMPLE_SIZE * sizeof(float), host_b, 0, NULL, NULL);

    size_t local_work_size = 256;
    size_t n_work_groups = (SAMPLE_SIZE + local_work_size + 1) / local_work_size;
    size_t global_work_size = n_work_groups * local_work_size;

    clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);

    clEnqueueReadBuffer(command_queue, device_c, CL_TRUE, 0, SAMPLE_SIZE * sizeof(float), host_c, 0, NULL, NULL);

    for (int i = 0; i < SAMPLE_SIZE; ++i) 
    {
        printf("[%d] = %.2f, \n", i, host_c[i]);
    }

    clReleaseMemObject(device_a);
    clReleaseMemObject(device_b);
    clReleaseMemObject(device_c);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseDevice(device_id);

    free(host_a);
    free(host_b);
    free(host_c);
}
