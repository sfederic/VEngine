#include "../Include/Common.hlsli"

VS_OUT main(VS_IN i)
{
    VS_OUT o;

    //@Todo: consolidate this code with TransformOut.hlsli
    
	//VERTEX BLENDING FOR SKINNED ANIMATION
    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = i.weights.x;
    weights[1] = i.weights.y;
    weights[2] = i.weights.z;
    weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

    float3 posL = float3(0.0f, 0.0f, 0.0f);
    //float3 normalL = float3(0.0f, 0.0f, 0.0f);
    if(isAnimated)
    {
        for (int index = 0; index < 4; ++index)
        {
		    //no nonuniform scaling
            posL += weights[index] * mul(boneTransforms[i.boneIndices[index]], float4(i.pos.xyz, 1.0f)).xyz;
            //normalL += weights[index] * mul((float3x3) boneTransforms[i.boneIndices[index]], i.normal);
        }
    }
    else
    {
        posL = i.pos.xyz;
    }

    //@Todo: shadow doesn't exist if animation isn't playing, because there are no bone matrices sent in.
    //Could probably fix this with a bool in a constant buffer to make 'posL' all 1s so the mul() doesn't fall over.
    o.pos = mul(model, float4(posL, 1.0f));
    o.pos = mul(lightViewProj, o.pos);
    o.posWS = mul(model, float4(posL, 1.0f));
    float4 newUv = mul(texMatrix, float4(i.uv, 0.f, 1.0f));
    o.uv = float2(newUv.x, newUv.y);
    o.normal = mul((float3x3) model, i.normal);
    o.shadowPos = mul(lightMVP, o.pos);
    o.instanceID = i.instanceID;
    o.tangent = i.tangent;
    o.colour = i.colour;

    return o;
}
