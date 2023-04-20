#include "../Include/Common.hlsli"

float4 main(VS_OUT i) : SV_TARGET
{
    float4 finalColour = t.Sample(s, i.uv);
    finalColour += sin(timeSinceStartup);
    return finalColour;
}
