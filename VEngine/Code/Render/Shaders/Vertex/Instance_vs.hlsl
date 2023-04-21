#include "../Include/TransformOut.hlsli"

VS_OUT main(VS_IN i)
{
	VS_OUT o = TransformOutInstance(i, instanceData[i.instanceID].modelMatrix);
	return o;
}

