__kernel void partition(__global unsigned long* data, int low, int high, __global int* pivot_index) 
{
    int i = low;
    int j = high;
    unsigned long pivot = data[(low + high) / 2];
    
    while (i <= j) 
    {
        while (data[i] < pivot) i++;
        while (data[j] > pivot) j--;
        
        if (i <= j) 
        {
            unsigned long temp = data[i];
            data[i] = data[j];
            data[j] = temp;
            i++;
            j--;
        }
    }
    
    *pivot_index = i;
}