#pragma once
#include <vector>
#include "System.h"

struct Material;

struct MaterialSystem : System
{
	std::vector<Material*> materials;

	//The material that is currently open in the materialeditor 
	Material* currentMaterialEditorInstance = nullptr;

	MaterialSystem();
	void AddMaterial(Material* material);
	void CreateAllMaterials();
	void Cleanup();
};

extern MaterialSystem materialSystem;
