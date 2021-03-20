#include "Include/CommonTypes.hlsl"

VS_OUT VSMain(VS_IN i)
{
	VS_OUT o;
	o.pos = mul(mvp, float4(i.pos, 1.0f));
	o.uv = i.uv;
	o.normal = mul(i.normal, (float3x3)model);

	return o;
}

float4 PSMain(VS_OUT i) : SV_Target
{
	float4 texColour = t.Sample(s, i.uv);
	float3 lightDir = float3(0.f, 1.f, 1.f);
	float diffuse = dot(-lightDir, i.normal);

	clip(texColour.a - 0.1f);

	//float4 finalColour = ambient + saturate(diffuse);
	return texColour * diffuse;
}