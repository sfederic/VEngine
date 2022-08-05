#include "../Include/Common.hlsli"

float4 main(VS_OUT i) : SV_Target
{
	return material.ambient;
}
