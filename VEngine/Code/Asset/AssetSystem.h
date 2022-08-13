#pragma once

#include "System.h"
#include "Render/RenderTypes.h"

struct AssetSystem : System
{
	AssetSystem();
	void WriteAllMeshDataToMeshAssetFiles();
	MeshData ReadAllMeshAssetsFromFile(const char* filename);
	void BuildAllGameplayMapFiles();
};

extern AssetSystem assetSystem;
