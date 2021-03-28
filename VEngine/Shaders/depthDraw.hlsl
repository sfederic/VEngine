//Shader for drawing actor with no depth (3D axis for example)
#include "Include/CommonTypes.hlsl"

VS_OUT VSMain(VS_IN i)
{
	VS_OUT o;
	o.pos = mul(mvp, float4(i.pos, 1.0f));
	o.pos.z = 0.001f; //Should be enough to give off the right effect for depth.
	o.uv = i.uv;
	o.normal = i.normal;

	return o;
}

float4 PSMain(VS_OUT i) : SV_Target
{
	float4 outColour = ambient;
	return outColour;
}