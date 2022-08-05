#include "../Include/PostProcess.hlsli"

VS_OUTPUT main(uint vertexID : SV_VertexID)
{
    VS_OUTPUT o;
    o.position = float4(positions[vertexID].xy, 0.0, 1.0);
    o.uv = float2(uvs[vertexID].x, -uvs[vertexID].y);
    return o;
}

