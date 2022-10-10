#include "../Include/Common.hlsli"

float4 main(VS_OUT i) : SV_Target
{
    float4 finalColour = instanceData[i.instanceID].colour;
    
    if (material.useTexture)
    {
        finalColour *= t.Sample(s, i.uv);
    }
    
    return finalColour;
}
