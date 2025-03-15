export module Render.TextureSystem;

import <string>;

export class Texture2D;

export namespace TextureSystem
{
	extern std::wstring selectedTextureInEditor;

	void CreateAllTextures();
	void Cleanup();
	Texture2D* FindTexture2D(std::string textureFilename);
	void RemoveTexture(std::string textureName);
};
