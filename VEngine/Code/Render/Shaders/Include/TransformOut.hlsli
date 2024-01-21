#include "Common.hlsli"

VS_OUT TransformOut(VS_IN i)
{
	VS_OUT o;

    o.colour = i.colour;
	o.pos = mul(mvp, float4(i.pos.xyz, 1.0f));
	o.posWS = mul(model, float4(i.pos.xyz, 1.0f));
	o.normal = mul((float3x3)invTranModel, i.normal);
    o.tangent = mul((float3x3)invTranModel, i.tangent);
		
	const float4 newUv = mul(texMatrix, float4(i.uv, 0.f, 1.0f));
    o.uv = float2(newUv.x, newUv.y);
		
	o.shadowPos = mul(lightMVP, o.posWS);
	o.instanceID = i.instanceID;

	return o;
}

VS_OUT TransformOutOutline(VS_IN i)
{
    VS_OUT o;

    o.colour = i.colour;
    i.pos.xyz += i.normal * 0.0065f;
    o.pos = mul(mvp, float4(i.pos.xyz, 1.0f));
    o.posWS = mul(model, float4(i.pos.xyz, 1.0f));
    o.normal = mul((float3x3) invTranModel, i.normal);
    o.tangent = mul((float3x3) invTranModel, i.tangent);
		
    const float4 newUv = mul(texMatrix, float4(i.uv, 0.f, 1.0f));
    o.uv = float2(newUv.x, newUv.y);
		
    o.shadowPos = mul(lightMVP, o.posWS);
    o.instanceID = i.instanceID;

    return o;
}

VS_OUT TransformOutInstance(VS_IN i, float4x4 modelMatrixFromInstanceData)
{
    VS_OUT o;
    
    const float4x4 viewProj = mul(proj, view);
    const float4x4 modelViewProj = mul(viewProj, modelMatrixFromInstanceData);

    o.pos = mul(modelViewProj, float4(i.pos.xyz, 1.0f));
    o.posWS = mul(model, float4(i.pos.xyz, 1.0f));
    o.uv = i.uv;
    o.normal = mul((float3x3)modelMatrixFromInstanceData, i.normal);
    o.shadowPos = float4(1.0f, 1.0f, 1.0f, 1.0f);
    o.instanceID = i.instanceID;
    o.tangent = i.tangent;
    o.colour = i.colour;
    
    return o;
}

VS_OUT TransformOutAnimation(VS_IN i)
{
    VS_OUT o;

	//VERTEX BLENDING FOR SKINNED ANIMATION
    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = i.weights.x;
    weights[1] = i.weights.y;
    weights[2] = i.weights.z;
    weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    for (int index = 0; index < 4; ++index)
    {
		//no nonuniform scaling
        posL += weights[index] * mul(boneTransforms[i.boneIndices[index]], float4(i.pos.xyz, 1.0f)).xyz;
        normalL += weights[index] * mul((float3x3) boneTransforms[i.boneIndices[index]], i.normal);
    }

    o.colour = i.colour;
    o.pos = mul(mvp, float4(posL, 1.0f));
    o.posWS = mul(model, float4(posL, 1.0f));
    float4 newUv = mul(texMatrix, float4(i.uv, 0.f, 1.0f));
    o.uv = float2(newUv.x, newUv.y);
    o.normal = mul((float3x3) model, normalL);
    o.shadowPos = mul(lightMVP, o.posWS);
    o.instanceID = i.instanceID;
    o.tangent = i.tangent;

    return o;
}
