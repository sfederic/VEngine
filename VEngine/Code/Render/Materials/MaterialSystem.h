#pragma once
#include <unordered_map>

struct Material;

struct MaterialSystem
{
	std::unordered_map<std::wstring, Material*> materialMap;

	void AddMaterial(Material* material);
	void RemoveMaterial(std::string materialName);

	//Find material based on the texture name a meshcomponent provides
	Material* Find(std::wstring textureName);

	void CreateAllMaterials();
};

extern MaterialSystem materialSystem;
