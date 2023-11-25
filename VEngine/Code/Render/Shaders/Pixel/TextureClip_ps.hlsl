#include "../Include/Common.hlsli"

float4 main(VS_OUT i) : SV_TARGET
{
    float4 texColour = defaultTexture.Sample(defaultSampler, i.uv);
    clip(texColour.a - 0.15f);
    
    texColour *= material.ambient;
    texColour *= i.colour;
    
    return texColour;
}
