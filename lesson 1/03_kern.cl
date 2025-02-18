__kernel void index_id(__global int* buffer, int n) 
{
    int gid = get_global_id(0);
    int lid = get_local_id(0);
    if(gid < n) 
    {
        buffer[gid] = lid;
    }
}

__kernel void backwards_kernel(__global int* buffer, int n) 
{
    int gid = get_global_id(0);
    if(gid < n) 
    {
        buffer[gid] = n - 1 - gid;
    }
}

__kernel void pair_switch(__global int* buffer, int n) 
{
    int pair_index = get_global_id(0);
    int i = pair_index * 2;

    if (i + 1 < n)
    {
        int tmp = buffer[i];
        buffer[i] = buffer[i + 1];
        buffer[i + 1] = tmp;
    }
}