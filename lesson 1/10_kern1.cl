__kernel void prime_test_single(__global int* flag, int n, int max_divisor)
{
    int divisor = get_global_id(0) + 2;
    if(divisor <= max_divisor)
    {
        if(n % divisor == 0)
            flag[divisor - 2] = 1;
        else
            flag[divisor - 2] = 0;
    }
}
