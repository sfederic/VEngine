//keep per object buffer for single draws
cbuffer cbPerObject : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
	float4x4 mvp;
	float4x4 texMatrix;
};

struct Material
{
	float4 ambient;
	float4 emissive;
	float4 diffuse;
	float4 specular;
	float2 uvOffset;
	float2 uvScale;
	float uvRotation;
	float specularPower;
	bool useTexture;

	float padding[1];
};

cbuffer cbMaterials : register(b1)
{
	Material material;
}

cbuffer cbSkiningData : register(b2)
{
	float4x4 boneTransforms[96];
};

struct Light
{
	float4 position;
	float4 direction;
	float4 colour;
	float spotAngle;
	float constantAtten;
	float linearAtten;
	float quadraticAtten;
	int lightType;
	bool enabled;
	int2 pad;
};

#define MAX_LIGHTS 8
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

cbuffer cbLights : register(b3)
{
	float4 eyePosition;
	float4 globalAmbient;
	int numLights;
	int3 pad;
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

float CalcAttenuation(Light light, float d)
{
	return 1.0f / (light.constantAtten + light.linearAtten * d + light.quadraticAtten * d * d);
}

LightingResult CalcPointLight(Light light, float3 V, float4 P, float3 N)
{
	LightingResult result = { 0.f, 0.f, 0.f, 0.f };

	float3 L = (light.position - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = CalcAttenuation(light, distance);

	result.diffuse = CalcDiffuse(light, L, N) * attenuation;
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
	float4 pos : SV_POSITION;
	float3 posWS : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

Texture2D t : register(t0);
SamplerState s : register(s0);
