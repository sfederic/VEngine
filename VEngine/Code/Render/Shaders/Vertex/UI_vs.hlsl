#include "../Include/Common.hlsli"

VS_OUT main(VS_IN i)
{
    VS_OUT o;
    o.pos = float4(i.pos.xyz, 1.0f);
    o.posWS = mul(model, float4(i.pos.xyz, 1.0f));
    o.uv = i.uv;
    o.normal = mul((float3x3) model, i.normal);
    o.instanceID = i.instanceID;
    o.shadowPos = float4(1.f, 1.f, 1.f, 1.f);
    o.tangent = i.tangent;
    o.colour = i.colour;
    return o;
}
