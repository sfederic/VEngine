#include "Include/CommonTypes.hlsli"

VS_OUT VSMain(VS_IN i)
{
	VS_OUT o;

	o.pos = mul(mvp, float4(i.pos, 1.0f));
	float4 newUv = mul(texMatrix, float4(i.uv, 0.f, 1.0f));
	o.uv = float2(newUv.x, newUv.y);
	o.normal = mul((float3x3)model, i.normal);

	return o;
}

float4 PSMain(VS_OUT i) : SV_Target
{
	float4 texColour = t.Sample(s, i.uv);
	float3 lightDir = directionalLight.direction;
	float diffuse = dot(-lightDir, i.normal);

	float4 baseAmbience = directionalLight.colour;

	//clip(texColour.a - 0.1f);
	float4 finalColour = (baseAmbience + diffuse) * texColour;
	return finalColour;
}
