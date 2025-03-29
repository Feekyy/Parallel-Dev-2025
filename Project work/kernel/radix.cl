#define QUICKSORT_THRESHOLD 16

void swap(__global unsigned long* a, __global unsigned long* b) 
{
    unsigned long temp = *a;
    *a = *b;
    *b = temp;
}

int partition(__global unsigned long* arr, int low, int high) 
{
    unsigned long pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) 
    {
        if (arr[j] < pivot) 
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quicksort(__global unsigned long* arr, int low, int high) 
{
    if (low < high) 
    {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

__kernel void radix_sort_kernel(__global unsigned long* input, __global unsigned long* output, __global int* count, unsigned int bit_shift, unsigned int size) 
{
    int gid = get_global_id(0);
    if (gid < size) 
    {
        unsigned int digit = (input[gid] >> bit_shift) & 0xF;
        atomic_inc(&count[digit]);
    }

    barrier(CLK_GLOBAL_MEM_FENCE);

    if (gid == 0 && size <= QUICKSORT_THRESHOLD) 
    {
        quicksort(input, 0, size - 1);
    }
}