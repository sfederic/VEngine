#pragma once
#include <unordered_map>
#include <string>

struct Texture2D;

struct TextureSystem
{
	std::unordered_map<std::wstring, Texture2D*> texture2DMap;

	void AddTexture2D(Texture2D* texture);
};
