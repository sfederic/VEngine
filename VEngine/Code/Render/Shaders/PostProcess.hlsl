#include "Include/CommonTypes.hlsli"
#include "Include/TransformOut.hlsli"

VS_OUT VSMain(VS_IN i)
{
	TransformOut transformOut;
	VS_OUT o = transformOut.Transform(i);
	o.pos = float4(i.pos.x + 1.f, -i.pos.y, 0.f, 1.0f);
	o.uv = i.uv;
	return o;
}

float4 PSMain(VS_OUT i) : SV_TARGET
{
	float4 final = t.Sample(s, i.uv);
	final.a = 0.2f;
	return final;
}
