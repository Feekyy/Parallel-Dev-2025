__kernel void find_missing(__global int* buffer, int n)
{
    int gid = get_global_id(0);
    if( gid > 0 && gid < n-1)
    {
        if (buffer[gid]== 0)
        {
            buffer[gid] = buffer[gid-1] + buffer[gid+1];
        }
    }
}
