#pragma once
#include <unordered_map>
#include <string>

struct Texture2D;

//Maps and manages various texture (2D, 3D, etc.) formats to their filenames on disk.
struct TextureSystem
{
	std::unordered_map<std::wstring, Texture2D*> texture2DMap;

	std::wstring selectedTextureInEditor;

	void AddTexture2D(Texture2D* texture);
	Texture2D* FindTexture2D(std::wstring textureFilename);
	void CreateAllTextures();
	void Cleanup();
};

extern TextureSystem textureSystem;
