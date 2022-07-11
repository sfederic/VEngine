#include "vpch.h"
#include "TextureSystem.h"
#include "Render/RenderUtils.h"
#include <filesystem>
#include "Log.h"

TextureSystem textureSystem;

Texture2D* TextureSystem::FindTexture2D(std::string textureFilename, bool useFullPath)
{
	//Set default texture if filename doesn't exist
	if (!useFullPath)
	{
		if (!std::filesystem::exists("Textures/" + textureFilename))
		{
			Log("%s not found.", textureFilename.c_str());
			textureFilename = "test.png";
		}
	}

	auto textureIt = texture2DMap.find(textureFilename);

	//Add texture2d to system
	if (textureIt == texture2DMap.end())
	{
		texture2DMap.insert(std::make_pair(textureFilename, std::make_unique<Texture2D>(textureFilename)));

		auto& texture = texture2DMap[textureFilename];

		if (systemState == SystemStates::Loaded)
		{
			RenderUtils::CreateTexture(texture.get(), useFullPath);
		}

		return texture.get();
	}

	return textureIt->second.get();
}

void TextureSystem::CreateAllTextures()
{
	for (auto& textureIt : texture2DMap)
	{
		RenderUtils::CreateTexture(textureIt.second.get());
	}

	systemState = SystemStates::Loaded;
}

void TextureSystem::Cleanup()
{
	texture2DMap.clear();

	systemState = SystemStates::Unloaded;
}
