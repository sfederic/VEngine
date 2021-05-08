#include "Include/CommonTypes.hlsl"

VS_OUT VSMain(VS_IN i)
{
	VS_OUT o;

	float4x4 world = modelMatrices[i.instanceID];
	float4x4 viewProj = mul(proj, view);
	float4x4 modelViewProj = mul(viewProj, world);

	o.pos = mul(modelViewProj, float4(i.pos, 1.0f));
	o.uv = i.uv;
	o.normal = mul((float3x3)world, i.normal);

	return o;
}

float4 PSMain(VS_OUT i) : SV_Target
{
	float4 texColour = t.Sample(s, i.uv);
	float3 lightDir = float3(0.f, 0.f, 1.f);
	float diffuse = dot(-lightDir, i.normal);

	//clip(texColour.a - 0.1f);
	//float4 finalColour = ambient + saturate(diffuse);
	return texColour;
}