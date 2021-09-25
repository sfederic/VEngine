#include "Include/CommonTypes.hlsli"

struct TransformOut
{
	VS_OUT Transform(VS_IN i)
	{
		VS_OUT o;

		o.pos = mul(mvp, float4(i.pos, 1.0f));
		o.posWS = mul(model, float4(i.pos, 1.0f));
		float4 newUv = mul(texMatrix, float4(i.uv, 0.f, 1.0f));
		o.uv = float2(newUv.x, newUv.y);
		o.normal = mul((float3x3)model, i.normal);
		o.shadowPos = mul(lightMVP, float4(i.pos, 1.0f));

		return o;
	}
};

VS_OUT VSMain(VS_IN i)
{
	TransformOut o;
	return o.Transform(i);
}

Texture2D diffuseMap;

void PSMain(VS_OUT i)
{
	float4 diffuse = diffuseMap.Sample(s, i.shadowPos.xy);
	clip(diffuse.a - 0.15f);
}
