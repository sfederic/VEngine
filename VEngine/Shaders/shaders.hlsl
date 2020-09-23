//#include "Include/CommonTypes.hlsl"

cbuffer cbPerObject : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
	float4x4 mvp;
};

cbuffer cbMaterial : register(b1)
{
	float4 ambient;
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
	o.uv = i.uv;
	o.normal = mul(i.normal, (float3x3)model);

	return o;
}

Texture2D t;
SamplerState s : register(s0);

float4 PSMain(VS_OUT i) : SV_Target
{
	float4 texColour = t.Sample(s, i.uv);
	float3 lightDir = float3(0.f, 1.f, 1.f);
	float diffuse = dot(-lightDir, i.normal);

	float4 finalColour = ambient + saturate(diffuse);

	return texColour * finalColour;
}