#include "vpch.h"
#include "MaterialSystem.h"
#include <cassert>
#include "Material.h"
#include "Serialiser.h"

MaterialSystem materialSystem;

Material* MaterialSystem::CreateMaterial(std::string textureFilename, ShaderItem& shaderItem)
{
	auto uid = GenerateUID();
	materials.emplace(uid, std::make_unique<Material>(textureFilename, shaderItem));
	auto material = materials[uid].get();
	material->SetUID(uid);
	return material;
}

void MaterialSystem::DestroyMaterial(UID materialUID)
{
	materials.erase(materialUID);
}

Material* MaterialSystem::FindMaterial(UID uid)
{
	auto materialIt = materials.find(uid);
	if (materialIt == materials.end())
	{
		return nullptr;
	}

	return materialIt->second.get();
}

Material MaterialSystem::LoadMaterialFromFile(const std::string filename)
{
	const std::string filepath = "Materials/" + filename;
	
	auto material = Material("test.png", ShaderItems::Default);
	auto materialProps = material.GetProps();

	Deserialiser d(filepath, OpenMode::In);
	d.Deserialise(materialProps);

	return material;
}

void MaterialSystem::CreateAllMaterials()
{
	for (auto& materialIt : materials)
	{
		materialIt.second->Create();
	}

	systemState = SystemStates::Loaded;
}

void MaterialSystem::Cleanup()
{
	materials.clear();

	systemState = SystemStates::Unloaded;
}
