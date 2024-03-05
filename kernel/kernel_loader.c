#include <kernel_loader.h>
#include <error.h>

#include <stdio.h>

#include <CL/cl.h>

char* createKernel()
{

    FILE *file = fopen("kern.cl", "r");

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* code = malloc(file_size + 1);

    fread(code, sizeof(char), file_size, file);
    code[file_size] = '\0';
    fclose(file);
    return code;
}

void buildProgram(Program* prog, char* kernel_code)
{
    prog->err = clGetPlatformIDs(1, &prog->pid, &prog->platforms);
	if (prog->err != CL_SUCCESS) 
    {
		printf("[ERROR]Error code: %d. Error: %s\n", prog->err, printError(prog->err));
	}

	prog->err = clGetDeviceIDs
    (
		prog->pid,
		CL_DEVICE_TYPE_GPU,
		1,
		&prog->did,
		&prog->devices
	);
	if (prog->err != CL_SUCCESS) 
    {
		printf("[ERROR] Error code: %d. Error: %s!\n", prog->err, printError(prog->err));	
    }

    prog->context = clCreateContext(NULL, prog->devices, &prog->did, NULL, NULL, NULL);

    prog->program = clCreateProgramWithSource(prog->context, 1, &kernel_code, NULL, NULL);
    prog->err = clBuildProgram(prog->program, 0, NULL, NULL, NULL, NULL);
    if (prog->err != CL_SUCCESS) 
    {
        printf("Build error! Code: %d\n", prog->err);
    }
}