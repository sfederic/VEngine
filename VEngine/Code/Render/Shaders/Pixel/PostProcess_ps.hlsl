#include "../Include/PostProcess.hlsli"

float4 main(VS_OUTPUT i) : SV_Target
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
