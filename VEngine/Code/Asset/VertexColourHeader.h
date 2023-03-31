#pragma once

#include <vector>
#include <DirectXMath.h>
#include "Core/UID.h"

struct VertexColourHeader
{
	UID meshComponentUID = 0;
	std::vector<DirectX::XMFLOAT4> colours;
};
