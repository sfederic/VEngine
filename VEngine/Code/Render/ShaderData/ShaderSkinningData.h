#pragma once

#include <DirectXMath.h>

struct ShaderSkinningData
{
	inline static const int MAX_SKINNING_DATA = 96;
	DirectX::XMMATRIX skinningMatrices[MAX_SKINNING_DATA]{};
	int isAnimated = false;
};
