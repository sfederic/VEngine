//Bobs actor/mesh up and down. Meant to be used on water or whatever.

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

	//mainly bob up on y-axis, give x and z small offset for effect to look 'natural'
	i.pos.y += sin(timeSinceStartup) * 0.1f;
	i.pos.x += sin(timeSinceStartup) * 0.05f;
	i.pos.z += sin(timeSinceStartup) * 0.05f;

	o.pos = mul(mvp, float4(i.pos, 1.0f));

	return o;
}

float4 PSMain(VS_OUT i) : SV_Target
{
	return t.Sample(s, i.uv);
}