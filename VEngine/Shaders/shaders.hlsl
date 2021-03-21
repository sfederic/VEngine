#include "Include/CommonTypes.hlsl"

VS_OUT VSMain(VS_IN i)
{
	VS_OUT o;
	//o.pos = mul(mvp, float4(i.pos, 1.0f));
	o.uv = i.uv;
	//o.normal = mul(i.normal, (float3x3)model);

	float weights[4] = { 0.f, 0.f, 0.f, 0.f };
	weights[0] = i.boneWeight.x;
	weights[1] = i.boneWeight.y;
	weights[2] = i.boneWeight.z;
	weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);
	
	for (int boneIndex = 0; boneIndex < 4; boneIndex++) 
	{
		posL += weights[boneIndex] * mul(float4(i.pos, 1.0f), boneTransforms[i.boneIndex[boneIndex]]).xyz;
		normalL += weights[boneIndex] * mul(i.normal, (float3x3)boneTransforms[i.boneIndex[boneIndex]]);
	}

	o.pos = mul(float4(posL, 1.0f), model);
	o.normal = mul(normalL, (float3x3)model);

	return o;
}

float4 PSMain(VS_OUT i) : SV_Target
{
	float4 texColour = t.Sample(s, i.uv);
	float3 lightDir = float3(0.f, 1.f, 1.f);
	float diffuse = dot(-lightDir, i.normal);

	//clip(texColour.a - 0.1f);

	//float4 finalColour = ambient + saturate(diffuse);
	return texColour * diffuse;
}