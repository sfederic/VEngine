cbuffer ShaderPostProcessData : register(b0)
{
    float gamma;
    float exposure;
    float2 shaderPostProcessDataPadding;
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

//Ref:https://www.gdcvault.com/play/1012351/Uncharted-2-HDR
float3 NaughtDogFilmicToneMap(float3 outColour)
{
    const float shoulderStrength = 0.22;
    const float linearStrength = 0.3;
    const float linearAngle = 0.1;
    const float toeStrenth = 0.2;
    const float toeNumerator = 0.01;
    const float toeDenominator = 0.3;
    const float linearWhitePointValue = 11.2;
    
    outColour = ((outColour * (shoulderStrength * outColour + linearAngle * linearStrength)
        + toeStrenth * toeNumerator) / (outColour * (shoulderStrength * outColour + linearStrength)
        + toeStrenth * toeDenominator)) - toeNumerator / toeDenominator;

    return outColour / linearWhitePointValue;
}

float3 ToneMapping(float3 HDRColor)
{
    // reinhard tone mapping
    //float3 mapped = HDRColor / (HDRColor + float3(1.f, 1.f, 1.f));
    
    // exposure tone mapping
    float3 mapped = float3(1.0, 1.0, 1.0) - exp(-HDRColor * exposure);
    
    // gamma correction 
    mapped = pow(mapped, float3(1.0 / gamma, 1.0 / gamma, 1.0 / gamma));
  
    return mapped;
}

float4 PSMain(VS_OUTPUT i) : SV_Target
{
    float3 colour = HDRTexture.Sample(s, i.uv).xyz;
    colour = NaughtDogFilmicToneMap(colour);
    return float4(colour, 1.0);
}