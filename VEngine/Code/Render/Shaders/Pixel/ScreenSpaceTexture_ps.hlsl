#include "../Include/Common.hlsli"

float4 main(VS_OUT i) : SV_TARGET
{
    //A lot of implementations online didn't really work, which is surprising. This Godot snippet came through.
    //Ref: https://godotshaders.com/shader/screenspace-tiled-texturing/
    
    //@Todo: this tiling scale variable will probably be fine for most effects, but it does bring into question
    //a setup for per-shader unique constant buffer mappings. In my mind it would probably require a custom 
    //shader class that can have constant buffer stages defined within it, that or just do the cheap option
    //and have extra constant buffers that are only populated when a ShaderItem's strings match.
    const float tilingScale = 256.0;
    
    const float2 uv = i.pos.xy * -1.0; //flip the uvs
    float2 tilingUV = uv / (tilingScale * -1.0);
    const float4 finalColour = defaultTexture.Sample(defaultSampler, tilingUV);
    return finalColour;
}