#include "../Include/Common.hlsli"

float4 main(VS_OUT i) : SV_TARGET
{
    float3 irradiance = CalcSHIrradiance(normalize(i.normal), lightProbeInstanceData[i.instanceID].SH);
    return float4(irradiance * PI * 4, 1.0);
}
