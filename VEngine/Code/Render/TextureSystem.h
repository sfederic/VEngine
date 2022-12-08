#pragma once

#include <string>

class Texture2D;

namespace TextureSystem
{
	extern std::wstring selectedTextureInEditor;

	void CreateAllTextures();
	void Cleanup();
	Texture2D* FindTexture2D(std::string textureFilename);
};
