__kernel void max_sort_kernel(__global unsigned long* arr, unsigned int size, unsigned int start) 
{
    int gid = get_global_id(0);
    int i = start + gid;
    
    if (i < size - 1) 
    {
        for (int j = i + 1; j < size; j++) 
        {
            if (arr[j] > arr[i]) 
            {
                unsigned long temp = arr[j];
                arr[j] = arr[i];
                arr[i] = temp;
            }
        }
    }
}