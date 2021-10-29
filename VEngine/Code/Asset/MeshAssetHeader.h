#pragma once
#include <cstdint>

//Binary header structure for mesh asset files
struct MeshAssetHeader
{
	uint64_t vertexCount = 0;
	uint64_t indexCount = 0;
};
