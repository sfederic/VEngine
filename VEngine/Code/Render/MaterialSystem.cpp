#include "vpch.h"
#include "MaterialSystem.h"
#include <cassert>
#include "Material.h"
#include "Core/Serialiser.h"
#include "Render/ShaderItem.h"
#include "Core/SystemStates.h"

static SystemStates systemState = SystemStates::Unloaded;
std::unordered_map<UID, Material*> materials;
std::string MaterialSystem::selectedMaterialInEditor;

void MaterialSystem::Init()
{
	Material::SetupBlendAndRastStateValues();
}

Material* MaterialSystem::CreateMaterial(std::string textureFilename, ShaderItem* shaderItem)
{
	auto uid = GenerateUID();
	materials.emplace(uid, new Material(textureFilename, shaderItem));
	auto material = materials.find(uid)->second;
	material->SetUID(uid);
	return material;
}

void MaterialSystem::DestroyMaterial(UID materialUID)
{
	delete materials.find(materialUID)->second;
	materials.erase(materialUID);
}

Material* MaterialSystem::FindMaterial(UID uid)
{
	auto materialIt = materials.find(uid);
	if (materialIt == materials.end())
	{
		return nullptr;
	}

	return materialIt->second;
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

void MaterialSystem::Cleanup()
{
	materials.clear();
	systemState = SystemStates::Unloaded;
}
