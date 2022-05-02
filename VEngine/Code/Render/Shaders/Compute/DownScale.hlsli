Texture2D HDRTex : register(t0);
RWStructuredBuffer<float> AverageLum : register(u0);
RWStructuredBuffer<float> AverageValues1D : register(u1);
groupshared float SharedPositions[1024];

static const float4 LUM_FACTOR = float4(0.299, 0.587, 0.114, 0);

cbuffer DownScaleConstants : register(b0)
{
    // Resolution of the down scaled target: x - width, y - height
    uint2 Res : packoffset(c0);
    // Total pixel in the downscaled image
    uint Domain : packoffset(c0.z);
    // Number of groups dispached on the first pass
    uint GroupSize : packoffset(c0.w);
}
