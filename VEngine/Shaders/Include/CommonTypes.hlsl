cbuffer cbPerObject : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
	float4x4 mvp;
};

cbuffer cbSkiningData : register(b1)
{
	float4x4 boneTransforms[96];
};

struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 boneWeight : BONEWEIGHT;
	uint4 boneIndex : BONEINDEX;
};

struct VS_OUT
{
	float4 pos: SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

Texture2D t : register(t0);
SamplerState s : register(s0);