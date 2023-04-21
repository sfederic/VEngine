#include "../Include/TransformOut.hlsli"

VS_OUT main(VS_IN i)
{
	VS_OUT o = TransformOutAnimation(i);
    return o;
}
