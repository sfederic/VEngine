#include "Include/CommonTypes.hlsli"

VS_OUT VSMain(VS_IN i)
{
	VS_OUT o;

	o.pos = mul(mvp, float4(i.pos, 1.0f));
	o.posWS = mul(model, float4(i.pos, 1.0f));
	o.uv = i.uv;
	o.normal = mul((float3x3)model, i.normal);
	o.shadowPos = float4(1.f, 1.f, 1.f, 1.f);
	o.instanceID = i.instanceID;

	return o;
}

float4 PSMain(VS_OUT i) : SV_Target
{
	return material.ambient;
}
