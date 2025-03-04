__kernel void multi_member(__global int* buffer, __global int* output, int n)
{
    int gid = get_global_id(0);
    if( gid < n)
    {
        int count = 0;
        for (int i = 0; i < n; i++)
        {
            if (buffer[i] == buffer[gid])
            {
                count++;
            }
        }
        output[gid] = count;
    }
}