#include "generate.h"
#include "sorting.h"
#include "opencl.h"

#include <CL/cl.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GROUP 2
#define SIZE 10
#define MAX 100



int main(int argc, char *argv[])
{
    clock_t start, end;
    double cpu_time_used;
    unsigned long* myArray;

    // Max Sort (CPU)
    /*myArray = generateRandomArray(SIZE, 0, MAX);
    if (myArray == NULL) 
    {
        printf("Memory allocation failed in generateRandomArray\n");
        return 1;
    }
    writeArrayToFile(myArray, SIZE, "maxdef.txt");
    start = clock();
    max_sort(myArray, SIZE);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Max Sort (CPU) time: %.6f seconds\n", cpu_time_used);
    writeArrayToFile(myArray, SIZE, "maxsort.txt");*/

    // Max Sort (OpenCL)
    myArray = generateRandomArray(SIZE, 0, MAX);
    if (myArray == NULL) 
    {
        printf("Memory allocation failed in generateRandomArray\n");
        return 1;
    }
    writeArrayToFile(myArray, SIZE, "maxdef_cl.txt");
    start = clock();
    cl_max_sort(myArray, SIZE, GROUP);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Max Sort (OpenCL) time: %.6f seconds\n", cpu_time_used);
    writeArrayToFile(myArray, SIZE, "maxsort_cl.txt");

    // Quick Sort (CPU)
    /*myArray = generateRandomArray(SIZE, 0, MAX);
    if (myArray == NULL) 
    {
        printf("Memory allocation failed in generateRandomArray\n");
        return 1;
    }
    writeArrayToFile(myArray, SIZE, "quickdef.txt");
    start = clock();
    quick_sort(myArray, 0, SIZE - 1);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Quick Sort (CPU) time: %.6f seconds\n", cpu_time_used);
    writeArrayToFile(myArray, SIZE, "quicksort.txt");*/

    // Quick Sort (OpenCL)
    myArray = generateRandomArray(SIZE, 0, MAX);
    if (myArray == NULL) 
    {
        printf("Memory allocation failed in generateRandomArray\n");
        return 1;
    }
    writeArrayToFile(myArray, SIZE, "quickdef_cl.txt");
    start = clock();
    cl_quick_sort(myArray, SIZE);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Quick Sort (OpenCL) time: %.6f seconds\n", cpu_time_used);
    writeArrayToFile(myArray, SIZE, "quicksort_cl.txt");

    // Radix Sort (CPU)
    /*myArray = generateRandomArray(SIZE, 0, MAX);
    if (myArray == NULL) 
    {
        printf("Memory allocation failed in generateRandomArray\n");
        return 1;
    }
    writeArrayToFile(myArray, SIZE, "radixdef.txt");
    start = clock();
    radix_sort_quick(myArray, SIZE, MAX);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Radix Sort (CPU) time: %.6f seconds\n", cpu_time_used);
    writeArrayToFile(myArray, SIZE, "radixsort.txt");*/

    // Radix Sort (OpenCL)
    myArray = generateRandomArray(SIZE, 0, MAX);
    if (myArray == NULL) 
    {
        printf("Memory allocation failed in generateRandomArray\n");
        return 1;
    }
    writeArrayToFile(myArray, SIZE, "radixdef_cl.txt");
    start = clock();
    cl_radix_sort(myArray, SIZE, MAX);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Radix Sort (OpenCL) time: %.6f seconds\n", cpu_time_used);
    writeArrayToFile(myArray, SIZE, "radixsort_cl.txt");

    free(myArray);

    return 0;
}

