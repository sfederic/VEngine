#include "../Include/Common.hlsli"

float4 main(VS_OUT i) : SV_Target
{
    float4 finalColour = instanceData[i.instanceID].colour;
    return finalColour;
}
