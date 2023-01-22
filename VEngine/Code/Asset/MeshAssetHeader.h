#pragma once

#include <cstdint>
#include "Animation/AnimationStructures.h"

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

//Binary header structure for skeletal animation asset files
struct AnimationAssetHeader
{
	char name[Animation::ANIM_NAME_MAX]{};
	uint64_t frameCount = 0;
};
