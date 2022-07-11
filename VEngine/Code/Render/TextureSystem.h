#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "Render/Texture2D.h"
#include "System.h"

struct TextureSystem : System
{
	std::unordered_map<std::string, std::unique_ptr<Texture2D>> texture2DMap;

public:
	std::wstring selectedTextureInEditor;

	TextureSystem() : System("TextureSystem") {}
	void CreateAllTextures();
	void Cleanup();

	//@Todo: this 'fullpath' stuff here on finding textures feels off.
	//Might be better to just always use the full path of the texture/asset.
	Texture2D* FindTexture2D(std::string textureFilename, bool useFullPath = false);
};

extern TextureSystem textureSystem;
