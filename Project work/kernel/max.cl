__kernel void max_step_kernel(__global const int* input, __global int* partial_max, int group_size, int size)
{
    int gid = get_global_id(0);
    int start = gid * group_size;
    int end = min(start + group_size, size);

    int max_val = input[start];
    for (int i = start + 1; i < end; i++)
    {
        if (input[i] > max_val)
        {
            max_val = input[i];
        }
    }

    partial_max[gid] = max_val;
}