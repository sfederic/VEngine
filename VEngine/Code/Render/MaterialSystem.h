#pragma once
#include <vector>
#include "SystemStates.h"

struct Material;

struct MaterialSystem
{
	std::vector<Material*> materials;

	SystemStates systemState = SystemStates::Unloaded;

	void AddMaterial(Material* material);
	void CreateAllMaterials();
	void Cleanup();
};

extern MaterialSystem materialSystem;
