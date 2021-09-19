#pragma once
#include <vector>
#include "SystemStates.h"

struct Material;

struct MaterialSystem
{
	std::vector<Material*> materials;

	SystemStates systemState = SystemStates::Unloaded;

	//The material that is currently open in the materialeditor 
	Material* currentMaterialEditorInstance = nullptr;

	MaterialSystem();
	void AddMaterial(Material* material);
	void CreateAllMaterials();
	void Cleanup();
};

extern MaterialSystem materialSystem;
