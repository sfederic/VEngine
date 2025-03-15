export module Render.MaterialSystem;

import Core.UID;

import <string>;

export class Material;

export namespace MaterialSystem
{
	extern std::string selectedMaterialInEditor;

	void Init();
	Material& CreateMaterial(std::string textureFilename, std::string shaderItemName);
	void DestroyMaterial(UID materialUID);
	Material* FindMaterial(UID uid);
	Material LoadMaterialFromFile(const std::string filename);
	void Cleanup();
};
