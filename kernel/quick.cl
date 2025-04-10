__kernel void partition(__global unsigned long* arr, int low, int high, __global int* pivot_index, unsigned long pivot) 
{
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) 
    {
        if (arr[j] <= pivot) 
        {
            i++;
            unsigned long temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    unsigned long temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    *pivot_index = i + 1;
}

__kernel void insertion_sort(__global unsigned long* arr, int low, int high) 
{
    for (int i = low + 1; i <= high; i++) 
    {
        unsigned long key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) 
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}