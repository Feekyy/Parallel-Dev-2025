__kernel void max_step_kernel(__global unsigned long* input, __global unsigned long* partial_max, unsigned int group_size, unsigned int size) 
{
    int gid = get_global_id(0);  
    int start = gid * group_size;

    if (start < size) 
    {
        unsigned long max_val = input[start];
        
        for (int i = 1; i < group_size && (start + i) < size; i++) 
        {
            if (input[start + i] > max_val) 
            {
                max_val = input[start + i];
            }
        }

        partial_max[gid] = max_val;  
    }
}

