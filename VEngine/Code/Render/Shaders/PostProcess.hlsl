#include "Include/Common.hlsli"
#include "Include/TransformOut.hlsli"

VS_OUT VSMain(VS_IN i)
{
	TransformOut transformOut;
	VS_OUT o = transformOut.Transform(i);
    o.pos = o.pos.xyww;
    return o;
}

float3 Rec709ToRec2020(float3 color)
{
	static const float3x3 conversion =
	{
		0.627402, 0.329292, 0.043306,
		0.069095, 0.919544, 0.011360,
		0.016394, 0.088028, 0.895578
	};
	return mul(conversion, color);
}

float4 PSMain(VS_OUT i) : SV_TARGET
{
	float4 final = t.Sample(s, i.uv);
	return final;
}
