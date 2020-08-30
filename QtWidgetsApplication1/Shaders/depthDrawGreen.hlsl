//Shader for drawing actor with no depth (3D axis for example)

cbuffer cbPerObject
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
	float4x4 mvp;
};

struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct VS_OUT
{
	float4 pos: SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

VS_OUT VSMain(VS_IN i)
{
	VS_OUT o;
	o.pos = mul(mvp, float4(i.pos, 1.0f));
	o.pos.z = 0.001f; //Should be enough to give off the right effect
	o.uv = i.uv;
	o.normal = i.normal;

	return o;
}

Texture2D t;
SamplerState s : register(s0);

float4 PSMain(VS_OUT i) : SV_Target
{
	float4 outColour = float4(0.1f, 0.9f, 0.1f, 1.f);
	return outColour;
}