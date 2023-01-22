#pragma once

#include "Render/RenderTypes.h"

namespace AssetSystem
{
	void WriteAllMeshDataToMeshAssetFiles();
	MeshData ReadAllMeshAssetsFromFile(const char* filename);
	void BuildAllGameplayMapFiles();
};
