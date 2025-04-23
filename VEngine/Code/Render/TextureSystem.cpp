#include "vpch.h"
#include "TextureSystem.h"
#include <filesystem>
#include "Core/SystemStates.h"
#include "Render/Texture2D.h"
#include "Core/Log.h"
#include "Asset/AssetBaseFolders.h"

static SystemStates systemState = SystemStates::Unloaded;
static std::unordered_map<std::string, std::unique_ptr<Texture2D>> texture2DMap;
std::wstring TextureSystem::selectedTextureInEditor;

Texture2D* TextureSystem::FindTexture2D(std::string textureFilename)
{
	//Set default texture if filename doesn't exist
	if (!std::filesystem::exists(AssetBaseFolders::texture + textureFilename))
	{
		Log("%s not found.", textureFilename.c_str());
		textureFilename = "test.png";
	}

	auto textureIt = texture2DMap.find(textureFilename);

	//Add texture2d to system
	if (textureIt == texture2DMap.end())
	{
		texture2DMap.emplace(textureFilename, std::make_unique<Texture2D>(textureFilename));

		auto& texture = texture2DMap[textureFilename];

		if (systemState == SystemStates::Loaded)
		{
			texture->Create();
		}

		return texture.get();
	}

	return textureIt->second.get();
}

void TextureSystem::RemoveTexture(std::string textureName)
{
	texture2DMap.erase(textureName);
}

void TextureSystem::CreateAllTextures()
{
	for (auto& [name, texture] : texture2DMap)
	{
		texture->Create();
	}

	systemState = SystemStates::Loaded;
}

void TextureSystem::Cleanup()
{
	texture2DMap.clear();

	systemState = SystemStates::Unloaded;
}
