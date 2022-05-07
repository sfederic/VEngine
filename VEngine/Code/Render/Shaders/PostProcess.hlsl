cbuffer ShaderPostProcessData : register(b0)
{
    float exposure;
    float shoulderStrength;
    float linearStrength;
    float linearAngle;
    float toeStrenth;
    float toeNumerator;
    float toeDenominator;
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

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

VS_OUTPUT VSMain(uint vertexID : SV_VertexID)
{
    VS_OUTPUT o;
    o.position = float4(positions[vertexID].xy, 0.0, 1.0);
    o.uv = float2(uvs[vertexID].x, -uvs[vertexID].y);
    return o;
}

//Ref:https://www.gdcvault.com/play/1012351/Uncharted-2-HDR
//Ref:http://filmicworlds.com/blog/filmic-tonemapping-operators/
float3 Uncharted2ToneMap(float3 outColour)
{    
    outColour = ((outColour * (shoulderStrength * outColour + linearAngle * linearStrength)
        + toeStrenth * toeNumerator) / (outColour * (shoulderStrength * outColour + linearStrength)
        + toeStrenth * toeDenominator)) - toeNumerator / toeDenominator;

    return outColour;
}

float4 PSMain(VS_OUTPUT i) : SV_Target
{
    float3 texColour = HDRTexture.Sample(s, i.uv).xyz;
    texColour *= exposure;
    
    float exposureBias = 2.0f;
    float3 curr = Uncharted2ToneMap(exposureBias * texColour);
    
    float3 whiteScale = 1.0f / Uncharted2ToneMap(linearWhitePointValue);
    
    float3 colour = curr * whiteScale;
    
    float3 finalColour = pow(colour, 1 / 2.2);
    return float4(finalColour, 1.0);
}