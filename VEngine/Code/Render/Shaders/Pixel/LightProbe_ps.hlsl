#include "../Include/Common.hlsli"

float4 main(VS_OUT i) : SV_TARGET
{
    float3 irradiance = CalcSHIrradiance(i.normal, SH);
    return float4(irradiance, 1.0);
}
