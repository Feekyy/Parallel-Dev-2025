#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CL/cl.h>

struct Program
{
    cl_device_id did;
    cl_uint platforms;
    cl_uint devices;
	cl_platform_id pid;
    cl_context context;
    cl_program program;
    int err;
};

int main(void)
{
    struct Program prog;
    size_t size = sizeof(prog);
    printf("%d", size);
    return 0;
}