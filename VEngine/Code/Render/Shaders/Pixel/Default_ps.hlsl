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

    float3 bitangent = cross(i.tangent, normal);
    float3x3 TBN = float3x3(i.tangent, bitangent, normal);
    //TBN = transpose(TBN);
    
    float3 bumpNormal = normalMap.Sample(s, i.uv).rgb;
    bumpNormal = bumpNormal * 2.0 - 1.0;
    bumpNormal = normalize(mul(TBN, bumpNormal));
    
    float3 V = normalize(eyePosition - position).xyz;

    LightingResult endResult = CalcForwardLighting(V, position, bumpNormal);

    endResult.diffuse *= material.ambient;
    endResult.specular *= material.ambient;

    float4 shadowColour = float4(0.f, 0.f, 0.f, 0.f);
    if (shadowsEnabled)
    {
        shadowColour = CalcShadowFactor(i.shadowPos);
        shadowColour /= 4.f;
    }
    
    //float4 shIrradiance = float4(GetSHIrradiance(i.normal, SH), 1.0f);
    
    float4 finalColour = ((globalAmbient + endResult.diffuse + endResult.specular) + shadowColour) * texColour;
    finalColour.a = material.ambient.a;
    return finalColour;
}
