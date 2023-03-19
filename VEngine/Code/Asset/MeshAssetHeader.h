#pragma once

#include <cstdint>

enum class SourceMeshFormat : uint8_t
{
	FBX
};

//Binary header structure for mesh asset files
struct MeshAssetHeader
{
	uint64_t vertexCount = 0;
	uint64_t indexCount = 0;
	uint64_t boneCount = 0;

	SourceMeshFormat sourceMeshFormat = SourceMeshFormat::FBX;
};
