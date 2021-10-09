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
	float3 normal = normalize(i.normal);
	float4 position = i.posWS;

	float3 V = normalize(eyePosition - position).xyz;

	LightingResult endResult = CalcForwardLighting(V, position, normal);

	endResult.diffuse = saturate(endResult.diffuse);

	//clip(texColour.a - 0.1f);

	float shadowColour = CalcShadowFactor(i.shadowPos);

	float4 localAmbient = float4(1.f, 1.f, 1.f, 1.0f);


	//float4 finalColour = (endResult.diffuse + globalAmbient) * texColour;
	float4 finalColour = (endResult.diffuse) * (texColour + shadowColour);
	return finalColour;
}
