__kernel void partition(__global int* arr, int low, int high, __global int* pivot_index, int pivot_digit, int exp, int is_radix) 
{
    int i = low - 1;
    int pivot = arr[high];
    
    for (int j = low; j <= high - 1; j++) 
    {
        int value = arr[j];
        int compare_val = is_radix ? (value / exp) % 10 : value;
        int pivot_compare = is_radix ? pivot_digit : pivot;

        if (compare_val > pivot_compare || (compare_val == pivot_compare && value > pivot)) 
        {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    *pivot_index = i + 1;
}