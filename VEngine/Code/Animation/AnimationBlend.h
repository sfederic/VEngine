#pragma once

#include <DirectXMath.h>

using namespace DirectX;

namespace InterpolateBlend
{
	XMVECTOR BlendRotation(XMVECTOR quat0, XMVECTOR quat1, float t)
	{
		return XMQuaternionSlerp(quat0, quat1, t);
	}

	XMVECTOR BlendTranslation(XMVECTOR pos0, XMVECTOR pos1, float t)
	{
		return XMVectorLerp(pos0, pos1, t);
	}

	XMVECTOR BlendScale(XMVECTOR scale0, XMVECTOR scale1, float t)
	{
		return XMVectorLerp(scale0, scale1, t);
	}
}
