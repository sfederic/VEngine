#include "../Include/Common.hlsli"

void main(VS_OUT i)
{
    float4 texColour = t.Sample(s, i.uv);
    clip(texColour.a - 0.15f);
}
