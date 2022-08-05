#include "Include/Common.hlsli"

float4 main(VS_OUT i) : SV_TARGET
{
    float4 texColour = t.Sample(s, i.uv);
    clip(texColour.a - 0.15f);
    return texColour;
}
