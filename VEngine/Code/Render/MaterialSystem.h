#pragma once

#include <string>
#include "UID.h"

class Material;
class ShaderItem;

namespace MaterialSystem
{
	extern std::string selectedMaterialInEditor;

	void Init();
	Material* CreateMaterial(std::string textureFilename, ShaderItem* shaderItem);
	void DestroyMaterial(UID materialUID);
	Material* FindMaterial(UID uid);
	Material LoadMaterialFromFile(const std::string filename);
	void CreateAllMaterials();
	void Cleanup();
};
