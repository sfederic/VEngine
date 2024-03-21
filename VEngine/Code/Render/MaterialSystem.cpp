#include "vpch.h"
#include "MaterialSystem.h"
#include "Material.h"
#include "Core/Serialiser.h"
#include "Render/ShaderItem.h"
#include "Core/SystemStates.h"

static SystemStates systemState = SystemStates::Unloaded;
std::unordered_map<UID, std::unique_ptr<Material>> materials;
std::string MaterialSystem::selectedMaterialInEditor;

void MaterialSystem::Init()
{
	Material::SetupBlendShaderItemsAndRastStateValues();
}

Material& MaterialSystem::CreateMaterial(std::string textureFilename, std::string shaderItemName)
{
	auto uid = GenerateUID();
	materials.emplace(uid, std::make_unique<Material>(textureFilename, shaderItemName));
	auto& material = materials.find(uid)->second;
	material->SetUID(uid);
	return *material;
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

	auto material = Material("test.png", "Default");
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
