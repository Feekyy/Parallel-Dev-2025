#ifndef KERNEL_H
#define KERNEL_H

#include <CL/cl.h>

typedef struct Program
{
    cl_device_id did;
    cl_uint platforms;
    cl_uint devices;
	cl_platform_id pid;
    cl_context context;
    cl_program program;
    int err;
} Program;

char* createKernel(char kern_name[20]);

void buildProgram(struct Program* prog, char* kernel_code);

#endif