static const float2 arrBasePos[6] =
{
    float2(-1.0, 1.0),
    float2(1.0, 1.0),
    float2(-1.0, -1.0),
    float2(-1.0, -1.0),
    float2(1.0, -1.0),
    float2(1.0, 1.0)
};

static const float2 arrUV[6] =
{
    float2(0.0, 0.0),
    float2(1.0, 0.0),
    float2(0.0, 1.0),
    float2(0.0, 1.0),
    float2(1.0, 0.0),
    float2(1.0, 1.0),
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD0;
};

VS_OUTPUT VSMain(uint VertexID : SV_VertexID)
{
    VS_OUTPUT Output;
    Output.Position = float4(arrBasePos[VertexID].xy, 0.0, 1.0);
    Output.UV = arrUV[VertexID].xy;

    return Output;
}

Texture2D<float4> HDRTexture : register(t0);
StructuredBuffer<float> AvgLum : register(t1);

SamplerState PointSampler : register(s0);

cbuffer FinalPassConstants : register(b0)
{
    float MiddleGrey : packoffset(c0);
    float LumWhiteSqr : packoffset(c0.y);
}

static const float4 LUM_FACTOR = float4(0.299, 0.587, 0.114, 0);

float3 ToneMapping(float3 HDRColor)
{
    // Find the luminance scale for the current pixel
    float LScale = dot(HDRColor, LUM_FACTOR.xyz);
    
    LScale *= MiddleGrey / AvgLum[0];
    LScale = (LScale + LScale * LScale / LumWhiteSqr) / (1.0 + LScale);
    
    // Apply the luminance scale to the pixels color
    return HDRColor * LScale;
}

float4 PSMain(VS_OUTPUT i) : SV_Target
{
    // Get the color sample
    float3 color = HDRTexture.Sample(PointSampler, i.UV.xy).xyz;
    
    // Tone mapping
    color = ToneMapping(color);
    
    // Output the LDR value
    return float4(color, 1.0);
}