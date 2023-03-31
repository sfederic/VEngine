#include "../Include/Common.hlsli"

VS_OUT main(VS_IN i)
{
	VS_OUT o;
	
	o.pos = mul(model, float4(i.pos.xyz, 1.0f));
	o.pos = mul(lightViewProj, o.pos);
	o.posWS = mul(model, float4(i.pos.xyz, 1.0f));
	float4 newUv = mul(texMatrix, float4(i.uv, 0.f, 1.0f));
	o.uv = float2(newUv.x, newUv.y);
	o.normal = mul((float3x3)model, i.normal);
	o.instanceID = i.instanceID;
    o.tangent = i.tangent;
    o.colour = i.colour;

    o.shadowPos = mul(lightMVP, float4(i.pos.xyz, 1.0f));

	return o;
}

