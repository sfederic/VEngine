struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

cbuffer ShaderPostProcessData : register(b0)
{
    float exposure;
    float linearAngle;
    float linearStrength;
    float shoulderStrength;
    float toeDenominator;
    float toeNumerator;
    float toeStrenth;
    float linearWhitePointValue;
};

Texture2D<float4> HDRTexture : register(t0);

SamplerState s : register(s0);

static const float2 positions[6] =
{
    float2(-1.f, -1.f),
	float2(-1.f, 1.f),
	float2(1.f, 1.f),
	float2(1.f, 1.f),
	float2(1.f, -1.f),
    float2(-1.f, -1.f)
};

static const float2 uvs[6] =
{
    float2(0.0, 0.0),
    float2(0.0, 1.0),
    float2(1.0, 1.0),
    float2(1.0, 1.0),
    float2(1.0, 0.0),
    float2(0.0, 0.0)
};

//Ref:https://www.gdcvault.com/play/1012351/Uncharted-2-HDR
//Ref:http://filmicworlds.com/blog/filmic-tonemapping-operators/
float3 Uncharted2ToneMap(float3 outColour)
{
    outColour = ((outColour * (shoulderStrength * outColour + linearAngle * linearStrength)
        + toeStrenth * toeNumerator) / (outColour * (shoulderStrength * outColour + linearStrength)
        + toeStrenth * toeDenominator)) - toeNumerator / toeDenominator;

    return outColour;
}
