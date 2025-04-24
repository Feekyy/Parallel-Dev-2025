#include "generate.h"
#include "sorting.h"
#include "opencl.h"

#include <CL/cl.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GROUP 1000

int main(int argc, char *argv[])
{
    clock_t start, end;
    double cpu_time_used;
    int* myArray;
    int size[] = {10000, 10000, 1000000, 1000000, 100000000, 100000000};
    int max[] = {15000, 32000, 15000, 32000, 15000, 32000};

    for (int i = 0; i < 5; i++)
    {
        printf("NEW CYCLE\nSize: %d, Max: %d\n\n", size[i], max[i]);

        // Max Sort (CPU)
        myArray = generateRandomArray(size[i], 0, max[i]);
        if (myArray == NULL) 
        {
            printf("Memory allocation failed in generateRandomArray\n");
            return 1;
        }
        writeArrayToFile(myArray, size[i], "maxdef.txt");
        start = clock();
        max_sort(myArray, size[i]);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Max Sort (CPU) time: %.6f seconds\n", cpu_time_used);
        writeArrayToFile(myArray, size[i], "maxsort.txt");

        // Max Sort (OpenCL)
        myArray = generateRandomArray(size[i], 0, max[i]);
        if (myArray == NULL) 
        {
            printf("Memory allocation failed in generateRandomArray\n");
            return 1;
        }
        writeArrayToFile(myArray, size[i], "maxdef_cl.txt");
        start = clock();
        cl_max_sort(myArray, size[i], GROUP);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Max Sort (OpenCL) time: %.6f seconds\n", cpu_time_used);
        writeArrayToFile(myArray, size[i], "maxsort_cl.txt");

        // Quick Sort (CPU)
        myArray = generateRandomArray(size[i], 0, max[i]);
        if (myArray == NULL) 
        {
            printf("Memory allocation failed in generateRandomArray\n");
            return 1;
        }
        writeArrayToFile(myArray, size[i], "quickdef.txt");
        start = clock();
        quick_sort(myArray, 0, size[i] - 1);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Quick Sort (CPU) time: %.6f seconds\n", cpu_time_used);
        writeArrayToFile(myArray, size[i], "quicksort.txt");

        // Quick Sort (OpenCL)
        myArray = generateRandomArray(size[i], 0, max[i]);
        if (myArray == NULL) 
        {
            printf("Memory allocation failed in generateRandomArray\n");
            return 1;
        }
        writeArrayToFile(myArray, size[i], "quickdef_cl.txt");
        start = clock();
        cl_quick_sort(myArray, size[i]);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Quick Sort (OpenCL) time: %.6f seconds\n", cpu_time_used);
        writeArrayToFile(myArray, size[i], "quicksort_cl.txt");

        // Radix Sort (CPU)
        myArray = generateRandomArray(size[i], 0, max[i]);
        if (myArray == NULL) 
        {
            printf("Memory allocation failed in generateRandomArray\n");
            return 1;
        }
        writeArrayToFile(myArray, size[i], "radixdef.txt");
        start = clock();
        radix_quicksort(myArray, size[i], max[i]);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Radix Sort (CPU) time: %.6f seconds\n", cpu_time_used);
        writeArrayToFile(myArray, size[i], "radixsort.txt");

        // Radix Sort (OpenCL)
        myArray = generateRandomArray(size[i], 0, max[i]);
        if (myArray == NULL) 
        {
            printf("Memory allocation failed in generateRandomArray\n");
            return 1;
        }
        writeArrayToFile(myArray, size[i], "radixdef_cl.txt");
        start = clock();
        cl_radix_sort(myArray, size[i], max[i]);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Radix Sort (OpenCL) time: %.6f seconds\n", cpu_time_used);
        writeArrayToFile(myArray, size[i], "radixsort_cl.txt");

        printf("\n");
    }

    free(myArray);

    return 0;
}

