#include "TextureSystem.h"
#include "PipelineObjects.h"
#include "Renderer.h"

TextureSystem textureSystem;

void TextureSystem::AddTexture2D(Texture2D* texture)
{
	texture2DMap[texture->filename] = texture;
}

Texture2D* TextureSystem::FindTexture2D(std::wstring textureFilename)
{
	auto textureIt = texture2DMap.find(textureFilename);
	if (textureIt == texture2DMap.end())
	{
		auto texture = new Texture2D(textureFilename);
		AddTexture2D(texture);
		return texture;
	}

	return textureIt->second;
}

void TextureSystem::CreateAllTextures()
{
	for (auto textureIt : texture2DMap)
	{
		Texture2D* texture = textureIt.second;
		texture = renderer.CreateTexture(textureIt.first);
	}
}
