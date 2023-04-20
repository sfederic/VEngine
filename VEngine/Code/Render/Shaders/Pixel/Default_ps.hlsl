#include "../Include/Common.hlsli"

float4 main(VS_OUT i) : SV_Target
{
    float4 texColour = float4(0.f, 0.f, 0.f, 0.f);
    
    if (material.useTexture)
    {
        texColour = t.Sample(s, i.uv);
        clip(texColour.a - 0.10f);
    }
    else
    {
        texColour = material.ambient;
    }

    float3 normal = normalize(i.normal);
    float4 position = i.posWS;

    //Put bump normal into CalcForwardLighting() if result is needed
    //float3 bumpNormal = CalcBumpNormalFromTangentBitangent(normal, i.tangent, i.uv);
    
    float3 V = normalize(eyePosition - position).xyz;

    LightingResult endResult = CalcForwardLighting(V, position, normal);

    endResult.diffuse *= material.ambient;
    endResult.specular *= material.ambient;

    float4 shadowColour = float4(0.f, 0.f, 0.f, 0.f);
    if (shadowsEnabled)
    {
        shadowColour = CalcShadowFactor(i.shadowPos);
        shadowColour /= 4.f;
    }
    
    //float4 lightMapColour = lightMap.Sample(s, i.lightMapUV);
    
    float4 finalColour = ((globalAmbient + endResult.diffuse + endResult.specular) + shadowColour) * texColour;
    
    if (isDiffuseProbeMapActive)
    {
        float4 shIrradiance = float4(GetSHIrradiance(i.normal, SH), 1.0f);
        finalColour *= shIrradiance * PI * 2;
    }
    
    finalColour.a = material.ambient.a;
    return i.colour * finalColour;
}
