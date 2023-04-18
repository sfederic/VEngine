#include "../Include/Common.hlsli"

float4 main(VS_OUT o) : SV_TARGET
{
    //float3 irradiance = GetSHIrradiance(o.normal, SH);
    //return float4(irradiance, 1.0);
    return float4(1, 0, 0, 1);
}
