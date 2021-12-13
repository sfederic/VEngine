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

	endResult.diffuse = saturate(endResult.diffuse);

	float shadowColour = CalcShadowFactor(i.shadowPos);

	float4 localAmbient = float4(0.3f, 0.3f, 0.3f, 1.0f);

	//grey colour 
	//float4 finalColour = endResult.diffuse * shadowColour;

	float4 finalColour = ((endResult.diffuse + localAmbient) * shadowColour) * texColour;

	return finalColour;
}
