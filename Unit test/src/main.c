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

    data.algorithm = max_sort_wrapper;
    data.save_filename = "max_sort_seq.txt";
    sort_time = runAlgorithm(&data);
    printf("Max Sort Seq Time: %f seconds\n", sort_time);

    data.algorithm = cl_max_sort_wrapper;
    data.save_filename = "max_sort_par.txt";
    sort_time = runAlgorithm(&data);
    printf("Max Sort Par Time: %f seconds\n", sort_time);

    data.algorithm = quick_sort_wrapper;
    data.save_filename = "quick_sort_seq.txt";
    sort_time = runAlgorithm(&data);
    printf("Quick Sort Seq Time: %f seconds\n", sort_time);

    data.algorithm = cl_quick_sort_wrapper;
    data.save_filename = "quick_sort_par.txt";
    sort_time = runAlgorithm(&data);
    printf("Quick Sort Par Time: %f seconds\n", sort_time);

    data.algorithm = radix_quicksort_wrapper;
    data.save_filename = "radix_sort_seq.txt";
    sort_time = runAlgorithm(&data);
    printf("Radix sort Seq Time: %f seconds\n", sort_time);

    data.algorithm = cl_radix_sort_wrapper;
    data.save_filename = "radix_sort_par.txt";
    sort_time = runAlgorithm(&data);
    printf("Radix sort Par Time: %f seconds\n", sort_time);

    printf("Program completed successfully.\n");
    return 0;
}

