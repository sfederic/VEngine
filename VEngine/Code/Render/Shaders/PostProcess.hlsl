#include "Include/CommonTypes.hlsli"
#include "Include/TransformOut.hlsli"

static const float2 arrBasePos[6] = {
	float2(-1.0, 1.0),
	float2(1.0, 1.0),
	float2(-1.0, -1.0),
	float2(1.0, 1.0),
	float2(1.0, -1.0),
	float2(-1.0, -1.0)
};

static const float2 arrUV[6] = {
	float2(0.0, 0.0),
	float2(1.0, 0.0),
	float2(0.0, 1.0),
	float2(1.0, 0.0),
	float2(1.0, 1.0),
	float2(0.0, 1.0)
}

VS_OUT VSMain(VS_IN i)
{
	VS_OUT o;
	TransformOut transformOut;
	o = transformOut.Transform(i);

	o.pos = arrBasePos[i.instanceID];
	o.uv = arrUV[i.instanceID];
}

float4 PSMain(VS_OUT i) : SV_TARGET
{
	return t.Sample(s, i.uv);
}
