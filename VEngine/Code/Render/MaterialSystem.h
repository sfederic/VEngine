#pragma once

#include <string>
#include "Core/UID.h"

class Material;

namespace MaterialSystem
{
	extern std::string selectedMaterialInEditor;

	void Init();
	Material& CreateMaterial(std::string textureFilename, std::string shaderItemName);
	void DestroyMaterial(UID materialUID);
	Material* FindMaterial(UID uid);
	Material LoadMaterialFromFile(const std::string filename);
	void Cleanup();
};
