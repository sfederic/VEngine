#pragma once

#include <vector>
#include <DirectXMath.h>
#include "Core/UID.h"

struct VertexColourData
{
	uint64_t numVertices = 0;
	UID meshComponentUID = 0;
	std::vector<DirectX::XMFLOAT4> colours;
};
