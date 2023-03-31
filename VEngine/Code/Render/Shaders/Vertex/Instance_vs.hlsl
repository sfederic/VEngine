#include "../Include/Common.hlsli"

VS_OUT main(VS_IN i)
{
	VS_OUT o;

	float4x4 world = instanceData[i.instanceID].modelMatrix;
	float4x4 viewProj = mul(proj, view);
	float4x4 modelViewProj = mul(viewProj, world);

	o.pos = mul(modelViewProj, float4(i.pos.xyz, 1.0f));
	o.posWS = mul(model, float4(i.pos.xyz, 1.0f));
	o.uv = i.uv;
	o.normal = mul((float3x3)world, i.normal);
	o.shadowPos = float4(1.0f, 1.0f, 1.0f, 1.0f);
	o.instanceID = i.instanceID;
    o.tangent = i.tangent;
    o.colour = i.colour;

	return o;
}

