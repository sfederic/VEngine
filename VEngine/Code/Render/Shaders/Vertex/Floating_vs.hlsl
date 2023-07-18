#include "../Include/TransformOut.hlsli"

VS_OUT main(VS_IN i)
{
    VS_OUT o = TransformOut(i);
    i.pos.y += sin(timeSinceStartup) * 0.1;
    o.pos = mul(mvp, float4(i.pos.xyz, 1.0));
    return o;
}
