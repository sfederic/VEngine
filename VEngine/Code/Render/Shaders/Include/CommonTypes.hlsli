//keep per object buffer for single draws
cbuffer cbPerObject : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
	float4x4 mvp;
	float4x4 texMatrix;
};

cbuffer cbMaterials : register(b1)
{
	float4 ambient;
	float2 uvOffset;
	float2 uvScale;
	float uvRotation;
	float padding[3];
}

cbuffer cbSkiningData : register(b2)
{
	float4x4 boneTransforms[96];
};

struct Light
{
	float4 position;
	float4 colour;
	float3 direction;
	float range;
	int lightType;
};

static const int MAX_LIGHTS = 8;
static const int DIRECTIONAL_LIGHT = 0;
static const int POINT_LIGHT = 1;
static const int SPOT_LIGHT = 2;

cbuffer cbLights : register(b3)
{
	float4 eyePosition;
	float4 globalAmbient;
	Light lights[MAX_LIGHTS];
}

float4 CalcDiffuse(Light light, float3 L, float3 N)
{
	float NdotL = max(0.0, dot(N, L));
	return light.colour * NdotL;
}

struct LightingResult
{
	float4 diffuse;
};

LightingResult CalcDirectionalLight(Light light, float3 normal)
{
	LightingResult result;

	float3 lightDir = -light.direction.xyz;
	result.diffuse = CalcDiffuse(light, lightDir, normal);

	return result;
}

struct InstanceData
{
	float4x4 modelMatrix;
};

StructuredBuffer<InstanceData> modelMatrices : register(t3);

struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	uint instanceID : SV_InstanceID;
};

struct VS_OUT
{
	float4 pos: SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

Texture2D t : register(t0);
SamplerState s : register(s0);
