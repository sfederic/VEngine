#include "Include/CommonTypes.h"

VS_OUT VSMain(VS_IN i)
{
	VS_OUT o;
	o.pos = mul(mvp, float4(i.pos, 1.0f));
	o.uv = i.uv;
	o.normal = i.normal;

	return o;
}

float4 PSMain(VS_OUT i) : SV_Target
{
	return float4(0.9f, 0.1f, 0.1f, 1.0f);
}