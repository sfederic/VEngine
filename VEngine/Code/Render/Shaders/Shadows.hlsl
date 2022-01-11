#include "Include/CommonTypes.hlsli"

VS_OUT VSMain(VS_IN i)
{
	VS_OUT o;

	o.pos = mul(model, float4(i.pos, 1.0f));
	o.pos = mul(lightViewProj, o.pos);
	o.posWS = mul(model, float4(i.pos, 1.0f));
	float4 newUv = mul(texMatrix, float4(i.uv, 0.f, 1.0f));
	o.uv = float2(newUv.x, newUv.y);
	o.normal = mul((float3x3)model, i.normal);
	o.instanceID = i.instanceID;

	o.shadowPos = mul(lightMVP, float4(i.pos, 1.0f));

	return o;
}

void PSMain(VS_OUT i)
{
	float4 diffuse = t.Sample(s, i.uv);
	clip(diffuse.a - 0.15f);
}
