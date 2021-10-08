#include "Include/CommonTypes.hlsli"

struct TransformOut
{
	VS_OUT Transform(VS_IN i)
	{
		VS_OUT o;

		o.pos = float4(i.pos, 1.0f);
		o.posWS = mul(model, float4(i.pos, 1.0f));
		o.uv = i.uv;
		o.normal = mul((float3x3)model, i.normal);

		o.shadowPos = float4(1.f, 1.f, 1.f, 1.f);

		return o;
	}
};

VS_OUT VSMain(VS_IN i)
{
	TransformOut t;
	return t.Transform(i);
}

float4 PSMain(VS_OUT i) : SV_Target
{
	float4 texColour = t.Sample(s, i.uv);
	clip(texColour.a - 0.15f);
	return texColour;
}
