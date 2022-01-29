#include "Include/CommonTypes.hlsli"
#include "Include/TransformOut.hlsli"

VS_OUT VSMain(VS_IN i)
{
	TransformOut o;
	return o.Transform(i);
}

float4 PSMain(VS_OUT i) : SV_Target
{
	float4 texColour = t.Sample(s, i.uv);
	clip(texColour.a - 0.10f);

	float3 normal = normalize(i.normal);
	float4 position = i.posWS;

	float3 V = normalize(eyePosition - position).xyz;

	LightingResult endResult = CalcForwardLighting(V, position, normal);

	float4 diffuse = endResult.diffuse * material.ambient;
	float4 specular = endResult.specular * material.specular;

	float4 shadowColour = float4(0.f, 0.f, 0.f, 0.f);
	if (shadowsEnabled)
	{
		shadowColour = CalcShadowFactor(i.shadowPos);
	}

	//good at debugging shadow visuals
	//float4 finalColour = shadowColour;

	float4 finalColour = ((diffuse + specular) + shadowColour) * texColour;
	return finalColour;
}
