#include "../Include/TransformOut.hlsli"

VS_OUT main(VS_IN i) 
{
    VS_OUT o = TransformOut(i);
	i.pos.y = sin((o.uv.x + o.uv.y) * timeSinceStartup) * 0.1f;
	o.pos = mul(mvp, float4(i.pos.xyz, 1.0f));
	return o;
}

