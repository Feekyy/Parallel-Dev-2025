__kernel void count_lower(__global int* input, __global int* output, int n)
{
    int gid = get_global_id(0);
    if (gid < n)
    {
        int count = 0;
        for (int i = 0; i < n; i++)
        {
            if (input[i] < input[gid])
            {
                count++;
            }
        }
        output[gid] = count;
    }
}