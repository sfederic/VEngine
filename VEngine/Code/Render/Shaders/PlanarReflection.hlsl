#include "Include/Common.hlsli"
#include "Include/TransformOut.hlsli"

VS_OUT VSMain(VS_IN i)
{
    TransformOut o;
    return o.Transform(i);
}

float4 PSMain(VS_OUT i) : SV_Target
{
    float4 textureColour = t.Sample(s, i.uv);
    
    float2 reflectTexCoord = float2(0.f, 0.f);
    reflectTexCoord.x = i.uv.x;
    reflectTexCoord.y = -i.uv.y;
    float4 reflectionColour = reflectionMap.Sample(s, reflectTexCoord);
    
    float3 normal = normalize(i.normal);
    float4 position = i.posWS;

    float3 V = normalize(eyePosition - position).xyz;

    LightingResult endResult = CalcForwardLighting(V, position, normal);

    endResult.diffuse *= material.ambient;
    endResult.specular *= material.ambient;

    float4 finalColour = (globalAmbient + endResult.diffuse + endResult.specular) * textureColour;
    
    return lerp(finalColour, reflectionColour, 0.2f);
}
