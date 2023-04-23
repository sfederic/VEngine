#include "../Include/Common.hlsli"

void main(VS_OUT i)
{
    float4 texColour = defaultTexture.Sample(defaultSampler, i.uv);
    clip(texColour.a - 0.15f);
}
