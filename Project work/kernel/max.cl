__kernel void max_step_kernel(__global ulong* input, __global ulong* partial_max, int group_size, int size) 
{
    int gid = get_global_id(0);
    int start = gid * group_size;

    if (start < size)
    {
        ulong max_val = input[start];

        for (int i = 0; i < group_size && (start + i) < size; i++)
        {
            ulong current = input[start + i];
            printf("gid %lu: input[%lu] = %lu\n", gid, start + i, current);
            
            if (current > max_val)
            {
                max_val = current;
            }
        }
        partial_max[gid] = max_val;
    }
    else
    {
        partial_max[gid] = 0;
    }
}
