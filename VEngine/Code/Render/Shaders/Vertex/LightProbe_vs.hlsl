#include "../Include/TransformOut.hlsli"

VS_OUT main(VS_IN i)
{
    VS_OUT o = TransformOutInstance(i, lightProbeInstanceData[i.instanceID].modelMatrix);
    return o;
}
