#include "../Include/Common.hlsli"

float4 main(VS_OUT i) : SV_Target
{
    const float4 texColour = GetTextureColour(i.uv);
    
    const float3 normal = normalize(i.normal);
    const float4 position = i.posWS;

    const float3 V = normalize(eyePosition - position).xyz;

    const LightingResult lightResult = CalcForwardLighting(V, position, normal);

    const float4 shadowFactor = GetShadowFactor(i.shadowPos);
    
    float4 finalColour = CalcFinalColour(i.colour, lightResult, shadowFactor, texColour);
    
    if (isDiffuseProbeMapActive)
    {
        const float4 shIrradiance = float4(CalcSHIrradiance(normal, SH), 1.0f);
        finalColour *= shIrradiance * PI * 2;
    }
    
    return finalColour;
}
