#include "MaterialSystem.h"
#include "Material.h"

MaterialSystem materialSystem;

void MaterialSystem::AddMaterial(Material* material)
{
    materialMap[material->textureFilename] = material;
}

void MaterialSystem::RemoveMaterial(std::string materialName)
{
}

Material* MaterialSystem::Find(std::wstring textureName)
{
    auto materialIt = materialMap.find(textureName);

    //create new material if texture doesn't exist.
    if (materialIt == materialMap.end())
    {
        Material* material = new Material(textureName, L"DefaultShader.hlsl");
        AddMaterial(material);
        return material;
    }

    return materialIt->second;
}

void MaterialSystem::CreateAllMaterials()
{
    for (auto materialIt : materialMap)
    {
        Material* mat = materialIt.second;
        mat->Create();
    }
}
