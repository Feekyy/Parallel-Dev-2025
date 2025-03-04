__kernel void find_minmax(__global const int* input, __global int* output, int n) 
{
    int gid = get_global_id(0);
    
    if (gid == 0) 
    {
        output[0] = input[0];
        output[1] = input[0];
        
        for (int i = 1; i < n; i++) 
        {
            if (input[i] < output[0]) 
            {
                output[0] = input[i];
            }
            if (input[i] > output[1]) 
            {
                output[1] = input[i];
            }
        }
    }
}