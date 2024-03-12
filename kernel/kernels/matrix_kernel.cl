__kernel void matrix
(
    global int* a,
    global int* b, 
    local int* c
) 
{
    int i = get_global_id(0)*2;
    int locali = get_local_id(0);
    int2 va = vload2(i, a);
    int2 vb = vload2(i+1, a);
    int2 vc = va + vb;
    c[locali] = vc[0] + vc[1];

    printf
    (
        "%d\t%d: %d + %d + %d + %d = %d\n", 
        i, 
        locali,
        va[0], va[1], vb[0], vb[1], 
        c[locali]
    );

}
