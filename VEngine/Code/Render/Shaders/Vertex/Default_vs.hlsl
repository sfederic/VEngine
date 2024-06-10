#include "../Include/TransformOut.hlsli"

VS_OUT main(VS_IN i)
{
    VS_OUT o = TransformOut(i);
    
    if (isDiffuseProbeMapActive)
    {
        const float3 normal = normalize(o.normal);
        const float4 shirradiance = float4(CalcSHIrradiance(normal, SH), 1.0f);
        o.colour += shirradiance * 4.0;
    }
    
	return o;
}


