#include "Include/CommonTypes.hlsli"

VS_OUT VSMain(VS_IN i)
{
	VS_OUT o;

	o.pos = mul(mvp, float4(i.pos, 1.0f));
	o.posWS = i.pos;
	float4 newUv = mul(texMatrix, float4(i.uv, 0.f, 1.0f));
	o.uv = float2(newUv.x, newUv.y);
	o.normal = mul((float3x3)model, i.normal);

	return o;
}

float4 PSMain(VS_OUT i) : SV_Target
{
	float4 texColour = t.Sample(s, i.uv);
	float3 normal = normalize(i.normal);
	float4 position = float4(i.posWS, 1.0f);

	float3 V = normalize(eyePosition - position).xyz;

	LightingResult endResult = CalcForwardLighting(V, position, normal);

	endResult.diffuse = saturate(endResult.diffuse);

	//clip(texColour.a - 0.1f);
	float4 finalColour = (endResult.diffuse + globalAmbient) * texColour;
	return finalColour;
}
