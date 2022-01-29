static const float SMAP_SIZE = 2048.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;
static const float PI = 3.14159265f;

cbuffer cbMatrices : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
	float4x4 mvp;
	float4x4 texMatrix;
	float4x4 lightMVP;
	float4x4 lightViewProj;
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

struct LightingResult
{
	float4 diffuse;
	float4 specular;
};

cbuffer cbLights : register(b3)
{
	float4 eyePosition;
	float4 globalAmbient;
	int numLights;
	bool shadowsEnabled;
	int2 pad;
	Light lights[MAX_LIGHTS];
}

cbuffer cbTime : register(b4)
{
	float deltaTime;
	float timeSinceStartup;
}

cbuffer cbMeshData : register(b5)
{
	float3 meshPosition;
}

float4 CalcDiffuse(Light light, float3 L, float3 N)
{
	float NdotL = max(0.0, dot(N, L));
	return (light.colour * NdotL) / PI;
}

float4 CalcSpecular(Light light, float3 V, float3 L, float3 N)
{
	[flatten]
	if (material.specularPower == 0.0)
	{
		float4 zeroSpecular = float4(0.f, 0.f, 0.f, 0.f);
		return zeroSpecular;
	}

	float3 R = normalize(reflect(-L, N));
	float RdotV = max(dot(V, R), 0.0);
	return light.colour * pow(RdotV, material.specularPower);
}

LightingResult CalcDirectionalLight(Light light, float3 view, float3 normal)
{
	LightingResult result;

	float3 lightDir = -light.direction.xyz;
	result.diffuse = CalcDiffuse(light, lightDir, normal);
	result.specular = CalcSpecular(light, view, lightDir, normal);

	return result;
}

float CalcAttenuation(Light light, float d)
{
	return 1.0f / (light.constantAtten + light.linearAtten * d + light.quadraticAtten * d * d);
}

LightingResult CalcPointLight(Light light, float3 V, float4 P, float3 N)
{
	LightingResult result;
	result.diffuse = float4(0.f, 0.f, 0.f, 0.f);
	result.specular = float4(0.f, 0.f, 0.f, 0.f);

	float3 L = (light.position - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = CalcAttenuation(light, distance);

	result.diffuse = CalcDiffuse(light, L, N) * attenuation;

	result.specular = CalcSpecular(light, V, L, N) * attenuation;

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
	result.specular = CalcSpecular(light, V, L, N) * attenuation * spotIntensity;

	return result;
}

LightingResult CalcForwardLighting(float3 V, float4 position, float3 normal)
{
	LightingResult endResult;
	endResult.diffuse = float4(0.f, 0.f, 0.f, 0.f);
	endResult.specular = float4(0.f, 0.f, 0.f, 0.f);

	[unroll]
	for (int i = 0; i < numLights; i++)
	{
		if (!lights[i].enabled)
		{
			continue;
		}

		LightingResult result;
		result.diffuse = float4(0.f, 0.f, 0.f, 0.f);
		result.specular = float4(0.f, 0.f, 0.f, 0.f);

		switch (lights[i].lightType)
		{
		case POINT_LIGHT:
			result = CalcPointLight(lights[i], V, position, normal);
			break;

		case SPOT_LIGHT:
			result = CalcSpotLight(lights[i], V, position, normal);
			break;

		case DIRECTIONAL_LIGHT:
			result = CalcDirectionalLight(lights[i], V, normal);
			break;
		}

		endResult.diffuse += result.diffuse;
		endResult.specular += result.specular;
	}

	endResult.diffuse = saturate(endResult.diffuse);
	endResult.specular = saturate(endResult.specular);

	return endResult;
}

struct InstanceData
{
	float4x4 modelMatrix;
	float4 colour;
};

StructuredBuffer<InstanceData> instanceData : register(t3);

struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 weights : WEIGHTS;
	uint4 boneIndices : BONEINDICES;
	uint instanceID : SV_InstanceID;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 posWS : POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
	float4 shadowPos : TEXCOORD1;
	uint instanceID : SV_InstanceID;
};

Texture2D t : register(t0);
Texture2D shadowMap : register(t1);
SamplerState s : register(s0);
SamplerComparisonState shadowSampler : register(s1);

float CalcShadowFactor(float4 shadowPos)
{
	shadowPos.xyz /= shadowPos.w;
	float depth = shadowPos.z;

	const float dx = SMAP_DX;
	float percentLit = 0.0f;

	const float2 offsets[9] =
	{
	float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
	float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
	float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
	};

	[unroll]
	for (int i = 0; i < 9; ++i)
	{
		percentLit += shadowMap.SampleCmpLevelZero(shadowSampler, shadowPos.xy + offsets[i], depth).r;
	}

	return percentLit /= 9.f;
}
