__kernel void find_minmax(__global const int* input, __global int* output, int n) 
{
    int gid = get_global_id(0);
    int lid = get_local_id(0);
    int group_size = get_local_size(0);
    
    __local int local_min[1];
    __local int local_max[1];
    
    if (lid == 0) 
    {
        local_min[0] = INT_MAX;
        local_max[0] = INT_MIN;
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    
    for (int i = gid; i < n; i += get_global_size(0)) 
    {
        int value = input[i];
        atomic_min(local_min, value);
        atomic_max(local_max, value);
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    
    if (lid == 0) 
    {
        atomic_min(&output[0], local_min[0]);
        atomic_max(&output[1], local_max[0]);
    }
}