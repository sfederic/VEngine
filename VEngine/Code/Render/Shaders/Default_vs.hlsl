#include "Include/Common.hlsli"
#include "Include/TransformOut.hlsli"

VS_OUT main(VS_IN i)
{
	TransformOut o;
	return o.Transform(i);
}

