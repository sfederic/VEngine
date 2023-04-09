#ifndef COMMON
#define COMMON

static const float SMAP_SIZE = 2048.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;
static const float PI = 3.14159265f;

struct InstanceData
{
    float4x4 modelMatrix;
    float4 colour;
};

struct VS_IN
{
    float4 colour : COLOUR;
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
    uint4 boneIndices : BONEINDICES;
    float3 weights : WEIGHTS;
    uint instanceID : SV_InstanceID;
};

struct VS_OUT
{
    float4 colour : COLOUR;
    float4 pos : SV_POSITION;
    float4 posWS : POSITION;
    float3 normal : NORMAL;
	float3 tangent : TANGENT0;
    float2 uv : TEXCOORD0;
	//float2 lightMapUV : LM_TEXCOORD;
    float4 shadowPos : TEXCOORD1;
    uint instanceID : SV_InstanceID;
};

Texture2D t : register(t0);
Texture2D shadowMap : register(t1);
Texture2D reflectionMap : register(t2);
StructuredBuffer<InstanceData> instanceData : register(t3);
TextureCube environmentMap : register(t4);
Texture2D normalMap : register(t5);
//Texture2D lightMap : register(t6);

SamplerState s : register(s0);
SamplerComparisonState shadowSampler : register(s1);

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
    float smoothness;
    float metallic;
	bool useTexture;
};

cbuffer cbMaterials : register(b1)
{
	Material material;
}

cbuffer cbSkiningData : register(b2)
{
	float4x4 boneTransforms[96];
    bool isAnimated;
};

struct Light
{
	float4 position;
	float4 direction;
	float4 colour;
	float spotAngle;
    float intensity;
	int lightType;
	bool enabled;
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
    float4 SH[9]; //Spherical Harmonics for probe linked to mesh
	float3 meshPosition;
    float cbMeshDataPad;
}

cbuffer ShaderMeshLightMapData : register(b6)
{
    int2 atlasSegmentOffset;
    int2 atlasSegmentSize;
    int2 atlasSize;
};

//Stole all this from https://interplayoflight.wordpress.com/2021/12/31/occlusion-and-directionality-in-image-based-lighting-implementation-details/ for simple diffuse testing.
//Was the only online example working with DXMath's SH lib.
float3 GetSHIrradiance(float3 n, float4 SH[9])
{
    float SQRT_PI = 1.7724538509f;
    float SQRT_5 = 2.2360679775f;
    float SQRT_15 = 3.8729833462f;
    float SQRT_3 = 1.7320508076f;
	
    float AO = 0.075f;
	
    float Y[9] =
    {
        1.0f / (2.0f * SQRT_PI),
        -SQRT_3 / (2.0f * SQRT_PI), 
        SQRT_3 / (2.0f * SQRT_PI),
        -SQRT_3 / (2.0f * SQRT_PI),
        SQRT_15 / (2.0f * SQRT_PI), 
        -SQRT_15 / (2.0f * SQRT_PI), 
        SQRT_5 / (4.0f * SQRT_PI),
        -SQRT_15 / (2.0f * SQRT_PI),
        SQRT_15 / (4.0f * SQRT_PI)
    };
	
    float t = acos(sqrt(1 - AO));

    float a = sin(t);
    float b = cos(t);
	
    float A0 = sqrt(4 * PI) * (sqrt(PI) / 2) * a * a;
    float A1 = sqrt(4 * PI / 3) * (sqrt(3 * PI) / 3) * (1 - b * b * b);
    float A2 = sqrt(4 * PI / 5) * (sqrt(5 * PI) / 16) * a * a * (2 + 6 * b * b);
 
    float3 irradiance =
        SH[0].xyz * A0 * Y[0] +
        SH[1].xyz * A1 * Y[1] * n.y +
        SH[2].xyz * A1 * Y[2] * n.z +
        SH[3].xyz * A1 * Y[3] * n.x +
        SH[4].xyz * A2 * Y[4] * (n.y * n.x) +
        SH[5].xyz * A2 * Y[5] * (n.y * n.z) +
        SH[6].xyz * A2 * Y[6] * (3.0 * n.z * n.z - 1.0) +
        SH[7].xyz * A2 * Y[7] * (n.z * n.x) +
        SH[8].xyz * A2 * Y[8] * (n.x * n.x - n.y * n.y);
 
    return max(irradiance, 0);
}

//Stole most of this PBR code from Frostbite https://www.ea.com/frostbite/news/moving-frostbite-to-pb
//because it's specular calcs looked the simplest.
//Some more good references from https://blog.selfshadow.com/publications/s2013-shading-course/#course_content (UE4, Blackops)
//And also the dudes at Tri-Ace https://research.tri-ace.com/
float3 F_Schlick(in float3 f0, in float u)
{
	return f0 + (1.f - f0) * pow(1.f - u , 5.f);
}

float V_SmithGGXCorrelated(float NdotL, float NdotV, float alphaG)
{
	float alphaG2 = alphaG * alphaG;
	float Lambda_GGXV = NdotL * sqrt((-NdotV * alphaG2 + NdotV) * NdotV + alphaG2);
	float Lambda_GGXL = NdotV * sqrt((-NdotL * alphaG2 + NdotL) * NdotL + alphaG2);
	return 0.5f / (Lambda_GGXV + Lambda_GGXL);
}

float D_GGX(float NdotH, float m)
{
	float m2 = m * m;
	float f = (NdotH * m2 - NdotH) * NdotH + 1;
	return m2 / (f * f);
}

float4 CalcDiffuse(float4 lightColour, float NdotL)
{
    return (lightColour * NdotL) / PI;
}

float4 CalcSpecularPBR(Light light, float NdotV, float NdotL, float NdotH, float LdotH, float HdotV)
{
    float smoothness = (1.f - material.smoothness) + 0.01f;

    float3 f0 = float3(0.0f, 0.0f, 0.0f);
    f0 = lerp(f0, material.ambient.xyz, material.metallic);
	
    float3 F = F_Schlick(f0, HdotV);
    float Vis = V_SmithGGXCorrelated(NdotV, NdotL, smoothness);
    float D = D_GGX(NdotH, smoothness);
    float3 Fr = D * F * Vis / PI;
    return float4(Fr, 1.0f) * light.colour;
}

float4 CalcSpecularBlinnPhong(Light light, float3 normal, float3 lightDir, float3 view, float specPower)
{
    float3 reflectDir = reflect(-lightDir, normal);
    float4 spec = pow(max(dot(view, reflectDir), 0.0), specPower);
    return spec * light.colour;
}

float CalcFalloff(float intensity, float distance)
{
    return intensity / max(pow(distance, 2.f), 0.001f);
}

LightingResult CalcDirectionalLight(Light light, float3 normal,
	float distance, float NdotV, float NdotL, float NdotH, float LdotH, float HdotV)
{
    LightingResult result;
    float lightDot = saturate(dot(light.direction.xyz, normal));
    result.diffuse = (light.colour * lightDot) / PI;
	result.specular = CalcSpecularPBR(light, NdotV, NdotL, NdotH, LdotH, HdotV);
	return result;
}

LightingResult CalcPointLight(Light light, float distance, float NdotV,
	float NdotL, float NdotH, float LdotH, float HdotV)
{
	float falloff = CalcFalloff(light.intensity, distance);

    LightingResult result;
    result.diffuse = CalcDiffuse(light.colour, NdotL) * falloff;
    result.specular = CalcSpecularPBR(light, NdotV, NdotL, NdotH, LdotH, HdotV) * falloff;
	return result;
}

float CalcSpotCone(Light light, float3 L)
{
	float minCos = cos(light.spotAngle);
	float maxCos = (minCos + 1.0f) / 2.0f;
	float cosAngle = dot(light.direction.xyz, -L);
	return smoothstep(minCos, maxCos, cosAngle);
}

LightingResult CalcSpotLight(Light light, float3 V, float4 P, float3 N, float3 L,
	float distance, float NdotV, float NdotL, float NdotH, float LdotH, float HdotV)
{
	LightingResult result;

	float falloff = CalcFalloff(light.intensity, distance);
	float spotIntensity = CalcSpotCone(light, L);

    result.diffuse = CalcDiffuse(light.colour, NdotL) * falloff * spotIntensity;
    result.specular = CalcSpecularPBR(light, NdotV, NdotL, NdotH, LdotH, HdotV) * falloff * spotIntensity;

	return result;
}

LightingResult CalcForwardLighting(float3 V, float4 position, float3 normal)
{
	LightingResult endResult;
	endResult.diffuse = float4(0.f, 0.f, 0.f, 0.f);
	endResult.specular = float4(0.f, 0.f, 0.f, 0.f);

    float NdotV = abs(dot(normal, V));

	[unroll]
	for (int i = 0; i < numLights; i++)
	{
		if (!lights[i].enabled)
		{
			continue;
		}

		//Dot products and vectors
        float3 L = (lights[i].position - position).xyz;
        float distance = length(L);
        L = L / distance;

        float3 H = normalize(V + L);

        float NdotL = saturate(dot(normal, L));
        float NdotH = saturate(dot(normal, H));
        float LdotH = saturate(dot(L, H));
        float HdotV = saturate(dot(H, V));
		
		LightingResult result;
		result.diffuse = float4(0.f, 0.f, 0.f, 0.f);
		result.specular = float4(0.f, 0.f, 0.f, 0.f);

		//Main light switch calc
		switch (lights[i].lightType)
		{
		case POINT_LIGHT:
			result = CalcPointLight(lights[i], distance, NdotV, NdotL, NdotH, LdotH, HdotV);
			break;

		case SPOT_LIGHT:
            result = CalcSpotLight(lights[i], V, position, normal, L, distance, NdotV, NdotL, NdotH, LdotH, HdotV);
			break;

		case DIRECTIONAL_LIGHT:
            result = CalcDirectionalLight(lights[i], normal, distance, NdotV, NdotL, NdotH, LdotH, HdotV);
			break;
		}

		endResult.diffuse += result.diffuse;
		endResult.specular += result.specular;
	}

	endResult.diffuse = saturate(endResult.diffuse);
	endResult.specular = saturate(endResult.specular);

	return endResult;
}

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

float3 CalcBumpNormalFromTangentBitangent(float3 normal, float3 tangent, float2 uv)
{
	float3 bitangent = cross(tangent, normal);
    float3x3 TBN = float3x3(tangent, bitangent, normal);
    TBN = transpose(TBN);
    
    float3 bumpNormal = normalMap.Sample(s, uv).rgb;
    bumpNormal = bumpNormal * 2.0 - 1.0;
    bumpNormal = normalize(mul(TBN, bumpNormal));
	
    return bumpNormal;
}

float InverseLerp(float x, float y, float value)
{
    return (value - x) / (y - x);
}

#endif 