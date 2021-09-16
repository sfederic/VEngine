cbuffer cbMatrices : register(b0)
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

static const int MAX_LIGHTS = 8;
static const int DIRECTIONAL_LIGHT = 0;
static const int POINT_LIGHT = 1;
static const int SPOT_LIGHT = 2;

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

float CalcSpotCone(Light light, float3 L)
{
	float minCos = cos(light.spotAngle);
	float maxCos = (minCos + 1.0f) / 2.0f;
	float cosAngle = dot(light.direction.xyz, -L);
	return smoothstep(minCos, maxCos, cosAngle);
}

LightingResult CalcSpotLight(Light light, float3 V, float4 P, float3 N)
{
	LightingResult result;

	float3 L = (light.position - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = CalcAttenuation(light, distance);
	float spotIntensity = CalcSpotCone(light, L);

	result.diffuse = CalcDiffuse(light, L, N) * attenuation * spotIntensity;

	return result;
}

LightingResult CalcForwardLighting(float3 V, float4 position, float3 normal)
{
	LightingResult endResult = { 0.f, 0.f, 0.f, 0.f };

	[unroll]
	for (int i = 0; i < numLights; i++)
	{
		LightingResult result = { 0.f, 0.f, 0.f, 0.f };

		switch (lights[i].lightType)
		{
		case POINT_LIGHT:
			result = CalcPointLight(lights[i], V, position, normal);
			break;

		case SPOT_LIGHT:
			result = CalcSpotLight(lights[i], V, position, normal);
			break;

		case DIRECTIONAL_LIGHT:
			result = CalcDirectionalLight(lights[i], normal);
			break;
		}

		endResult.diffuse += result.diffuse;
	}

	return endResult;
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
	float4 posWS : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

Texture2D t : register(t0);
SamplerState s : register(s0);
