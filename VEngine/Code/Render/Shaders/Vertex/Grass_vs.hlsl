#include "../Include/TransformOut.hlsli"

VS_OUT main(VS_IN i)
{
	VS_OUT o;

	float4 newUv = mul(texMatrix, float4(i.uv, 0.f, 1.0f));
	o.uv = float2(newUv.x, newUv.y);
    o.colour = i.colour;
	o.normal = mul((float3x3)model, i.normal);
	o.posWS = mul(model, float4(i.pos.xyz, 1.0f));
	o.shadowPos = mul(lightMVP, o.posWS);
	o.instanceID = i.instanceID;
    o.tangent = i.tangent;

	//Simple wave
	i.pos.z += (sin(timeSinceStartup) * i.pos.y) * 0.1f;
	i.pos.x += (sin(timeSinceStartup) * i.pos.y) * 0.1f;
	o.pos = mul(mvp, float4(i.pos.xyz, 1.0f));
	return o;
}
