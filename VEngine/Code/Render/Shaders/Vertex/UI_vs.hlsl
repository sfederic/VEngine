#include "../Include/TransformOut.hlsli"

VS_OUT main(VS_IN i)
{
    VS_OUT o = TransformOut(i);
    o.pos = float4(i.pos.xyz, 1.0f);
    return o;
}
