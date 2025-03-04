__kernel void prime_test_range(__global int* flag, int n, int start_divisor, int range)
{
    int gid = get_global_id(0);
    int local_start = start_divisor + gid * range;
    int local_end = local_start + range;
    
    int found = 0;
    for (int d = local_start; d < local_end; d++)
    {
        if(d > 1 && d <= n/2)
        {
            if(n % d == 0) {
                found = 1;
                break;
            }
        }
    }
    flag[gid] = found;
}