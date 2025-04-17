__kernel void partition(__global int* arr, int low, int high, __global int* pivot_index, int pivot_digit, int exp, int is_radix) 
{
    int i = low - 1;
    pivot_index[0] = i + 1;

    for (int j = low; j <= high - 1; j++) 
    {
        int value = arr[j];
        int compare_val = is_radix ? (value / exp) % 10 : value;

        if (compare_val >= pivot_digit) 
        {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    int value = arr[high];
    int compare_val = is_radix ? (value / exp) % 10 : value;

    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    pivot_index[0] = i + 1;
}