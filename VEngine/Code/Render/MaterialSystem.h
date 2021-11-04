#pragma once
#include <unordered_map>
#include <UID.h>
#include "System.h"

struct Material;

struct MaterialSystem : System
{
	std::unordered_map<UID, Material*> materials;

	//The material that is currently open in the materialeditor 
	Material* currentMaterialEditorInstance = nullptr;

	MaterialSystem();
	void AddMaterial(Material* material);
	Material* FindMaterial(UID uid);
	void CreateAllMaterials();
	void Cleanup();
};

extern MaterialSystem materialSystem;
