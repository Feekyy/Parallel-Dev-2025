#include "generate.h"
#include "sorting.h"
#include "parallel.h"

#include <CL/cl.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    double sort_time;

    test_data data;
    data.size = 10;
    data.min = 0;
    data.max = 100;

    deleteSavesFolder();
    createSavesFolder();

    printf("Starting Max Sort Sequential...\n");
    data.algorithm = max_sort_wrapper;
    sort_time = runAlgorithm(&data);
    data.save_filename = "max_sort_seq.txt";
    printf("Max Sort Seq Time: %f seconds\n", sort_time);

    printf("Starting Max Sort Parallel...\n");
    data.algorithm = cl_max_sort_wrapper;
    data.save_filename = "max_sort_par.txt";
    sort_time = runAlgorithm(&data);
    printf("Max Sort Par Time: %f seconds\n", sort_time);

    printf("Starting Quick Sort Sequential...\n");
    data.algorithm = quick_sort_wrapper;
    sort_time = runAlgorithm(&data);
    data.save_filename = "quick_sort_seq.txt";
    printf("Quick Sort Seq Time: %f seconds\n", sort_time);

    printf("Starting Quick Sort Parallel...\n");
    data.algorithm = cl_quick_sort_wrapper;
    sort_time = runAlgorithm(&data);
    data.save_filename = "quick_sort_par.txt";
    printf("Quick Sort Par Time: %f seconds\n", sort_time);

    printf("Starting Radix Sort Sequential...\n");
    data.algorithm = radix_quicksort_wrapper;
    sort_time = runAlgorithm(&data);
    data.save_filename = "radix_sort_seq.txt";
    printf("Radix sort Seq Time: %f seconds\n", sort_time);

    printf("Starting Radix Sort Parallel...\n");
    data.algorithm = cl_radix_sort_wrapper;
    sort_time = runAlgorithm(&data);
    data.save_filename = "radix_sort_par.txt";
    printf("Radix sort Par Time: %f seconds\n", sort_time);

    printf("Program completed successfully.\n");
    return 0;
}

