__kernel void moving_average(__global const float* input, __global float* output, int n, int radius)
{
    int gid = get_global_id(0);
    if(gid < n)
    {
        int start = (gid - radius) < 0 ? 0 : (gid - radius);
        int end   = (gid + radius) >= n ? n - 1 : (gid + radius);
        float sum = 0.0f;
        int count = 0;
        for (int i = start; i <= end; i++) 
        {
            sum += input[i];
            count++;
        }
        output[gid] = sum / count;
    }
}
