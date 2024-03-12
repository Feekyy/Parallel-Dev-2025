#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kernel_loader.h>

#include <CL/cl.h>

int main(void)
{
    int i, a, b;
    
    Program prog;

    buildProgram(&prog, createKernel("matrix_kernel.cl"));

    cl_kernel kernel = clCreateKernel(prog.program, "hello_kernel", NULL);

    int* host_buffer = (int*)malloc(sizeof(int)*3);

    cl_mem device_buffer = clCreateBuffer(prog.context, CL_MEM_READ_WRITE, sizeof(int)*3, NULL, NULL);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&device_buffer);
    clSetKernelArg(kernel, 1, sizeof(int), (void*)&SAMPLE_SIZE);

    cl_command_queue command_queue = clCreateCommandQueue(prog.context, prog.did, NULL, NULL);

    clEnqueueWriteBuffer
    (
        command_queue,
        device_buffer,
        CL_FALSE,
        0,
        SAMPLE_SIZE * sizeof(int),
        host_buffer,
        0,
        NULL,
        NULL
    );

    size_t local_work_size = 256;
    size_t n_work_groups = (SAMPLE_SIZE + local_work_size + 1) / local_work_size;
    size_t global_work_size = n_work_groups * local_work_size;

    clEnqueueNDRangeKernel(
        command_queue,
        kernel,
        1,
        NULL,
        &global_work_size,
        &local_work_size,
        0,
        NULL,
        NULL
    );

    clEnqueueReadBuffer(
        command_queue,
        device_buffer,
        CL_TRUE,
        0,
        SAMPLE_SIZE * sizeof(int),
        host_buffer,
        0,
        NULL,
        NULL
    );

    for (i = 0; i < SAMPLE_SIZE; ++i) 
    {
        printf("[%d] = %d, ", i, host_buffer[i]);
    }

    clReleaseKernel(kernel);
    clReleaseProgram(prog.program);
    clReleaseContext(prog.context);
    clReleaseDevice(prog.did);

    free(host_buffer);
}
