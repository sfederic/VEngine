#pragma once
#include "System.h"

struct AssetSystem : System
{
	AssetSystem();
	void WriteAllMeshDataToMeshAssetFiles();
	void ReadAllMeshAssetsFromFile();
	void BuildAllGameplayMapFiles();
};

extern AssetSystem assetSystem;
