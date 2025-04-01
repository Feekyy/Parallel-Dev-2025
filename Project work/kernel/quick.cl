__kernel void quicksort_iterative(__global unsigned long* data, int size) 
{
    int stack[256];
    int top = -1;

    stack[++top] = 0;
    stack[++top] = size - 1;

    while (top >= 0) 
    {
        int high = stack[top--];
        int low = stack[top--];

        unsigned long pivot = data[low + (high - low) / 2];

        int i = low;
        int j = high;

        while (i <= j) {
            while (data[i] < pivot) i++;
            while (data[j] > pivot) j--;
            if (i <= j) {
                unsigned long temp = data[i];
                data[i] = data[j];
                data[j] = temp;
                i++;
                j--;
            }
        }

        if (low < j) 
        {
            stack[++top] = low;
            stack[++top] = j;
        }

        if (i < high) 
        {
            stack[++top] = i;
            stack[++top] = high;
        }
    }
}
