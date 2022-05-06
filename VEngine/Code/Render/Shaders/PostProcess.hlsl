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
    float3 color = HDRTexture.Sample(s, i.uv).xyz;
    color = ToneMapping(color);
    return float4(color, 1.0);
}