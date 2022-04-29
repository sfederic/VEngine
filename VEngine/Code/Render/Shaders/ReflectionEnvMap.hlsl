#include "Include/Common.hlsli"
#include "Include/TransformOut.hlsli"

VS_OUT VSMain(VS_IN i)
{
    TransformOut o;
    return o.Transform(i);
}

float4 PSMain(VS_OUT i) : SV_Target
{
    float3 normal = normalize(i.normal);
    float4 texColour = environmentMap.Sample(s, normal);
    return texColour + globalAmbient;
}
