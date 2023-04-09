#include "Common.hlsli"

struct TransformOut
{
	VS_OUT Transform(VS_IN i)
	{
		VS_OUT o;

        o.colour = i.colour;
		o.pos = mul(mvp, float4(i.pos.xyz, 1.0f));
		o.posWS = mul(model, float4(i.pos.xyz, 1.0f));
		o.normal = mul((float3x3)model, i.normal);
        o.tangent = mul((float3x3)model, i.tangent);
		
		float4 newUv = mul(texMatrix, float4(i.uv, 0.f, 1.0f));
        o.uv = float2(newUv.x, 1.0 - newUv.y);
		
		//@Todo: the way the uvs are calculated here could be moved to a seperate UV variable in the Vertex struct
		//and built up during lightmap gen, written out to the mesh file format is these lerps get too expensive.
		
		//Light map uv offset calc (this took forever to figure out with the lerp() and inverse_lerp()).
		//Ref:https://gamedev.stackexchange.com/questions/110448/how-to-convert-uv-position-to-texture-atlas-pos-and-reapply-to-uv-in-unity
        //float xmin = InverseLerp(0, atlasSize.x, atlasSegmentOffset.x);
        //float ymin = InverseLerp(0, atlasSize.y, atlasSegmentOffset.y);
        //float xmax = InverseLerp(0, atlasSize.x, atlasSegmentOffset.x + atlasSegmentSize.x);
        //float ymax = InverseLerp(0, atlasSize.y, atlasSegmentOffset.y + atlasSegmentSize.y);
		
        //o.lightMapUV = float2(lerp(xmin, xmax, o.uv.x), lerp(ymin, ymax, o.uv.y));
		
		o.shadowPos = mul(lightMVP, o.posWS);
		o.instanceID = i.instanceID;

		return o;
	}

	VS_OUT TransformAnimation(VS_IN i)
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
			normalL += weights[index] * mul((float3x3)boneTransforms[i.boneIndices[index]], i.normal);
		}

        o.colour = i.colour;
		o.pos = mul(mvp, float4(posL, 1.0f));
		o.posWS = mul(model, float4(posL, 1.0f));
		float4 newUv = mul(texMatrix, float4(i.uv, 0.f, 1.0f));
		o.uv = float2(newUv.x, newUv.y);
		o.normal = mul((float3x3)model, normalL);
		o.shadowPos = mul(lightMVP, o.posWS);
		o.instanceID = i.instanceID;
        o.tangent = i.tangent;

		return o;
	}
};
