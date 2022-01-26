#include "Include/CommonTypes.hlsli"
#include "Include/TransformOut.hlsli"

VS_OUT VSMain(VS_IN i)
{
	VS_OUT o;

	float4 newUv = mul(texMatrix, float4(i.uv, 0.f, 1.0f));
	o.uv = float2(newUv.x, newUv.y);
	o.normal = mul((float3x3)model, i.normal);
	o.posWS = mul(model, float4(i.pos, 1.0f));
	o.shadowPos = mul(lightMVP, o.posWS);
	o.instanceID = i.instanceID;

	//Simple wave
	i.pos.z += (sin(timeSinceStartup) * i.pos.y) * 0.1f;
	i.pos.x += (sin(timeSinceStartup) * i.pos.y) * 0.1f;
	o.pos = mul(mvp, float4(i.pos, 1.0f));
	return o;
}

float4 PSMain(VS_OUT i) : SV_Target
{
	float4 texColour = t.Sample(s, i.uv);
	clip(texColour.a - 0.15f);
	return texColour;
}
