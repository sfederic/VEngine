cbuffer DownScaleConstants : register(b0)
{
    // Resolution of the down scaled target: x - width, y - height
    uint2 Res : packoffset(c0);
    // Total pixel in the downscaled image
    uint Domain : packoffset(c0.z);
    // Number of groups dispached on the first pass
    uint GroupSize : packoffset(c0.w);
}

[numthreads(1, 1, 1)]
void CSMain()
{
    
}
