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

float3 Rec709ToRec2020(float3 color)
{
    static const float3x3 conversion =
    {
        0.627402, 0.329292, 0.043306,
		0.069095, 0.919544, 0.011360,
		0.016394, 0.088028, 0.895578
    };
    return mul(conversion, color);
}

static const float4 LUM_FACTOR = float4(0.299, 0.587, 0.114, 0);

float3 FilmicToneMap()
{
    float3 outColour = 0.0;
    
    float3 ld = 0.002;
    float linReference = 0.18;
    float logReference = 444;
    float logGamma = 0.45;
    
    outColour = (log10(0.4 * outColour / linReference) / ld * logGamma + logReference) / 1023.f;
    outColour = clamp(outColour, 0, 1);
    
    return outColour;
}

//Same reference, GDC and blog pair
//https://www.gdcvault.com/play/1012351/Uncharted-2-HDR
//http://filmicworlds.com/blog/filmic-tonemapping-operators/
float3 NaughtDogFilmicToneMap(float3 outColour)
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
    float3 curr = NaughtDogFilmicToneMap(exposureBias * texColour);
    
    float3 whiteScale = 1.0f / NaughtDogFilmicToneMap(linearWhitePointValue);
    
    float3 colour = curr * whiteScale;
    
    float3 finalColour = pow(colour, 1 / 2.2);
    return float4(finalColour, 1.0);
}