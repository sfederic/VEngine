#pragma once
#include <string>

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

struct Texture2D
{
	std::string filename;

	ID3D11Resource* data = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;

	uint32_t width = 0;
	uint32_t height = 0;

	Texture2D() {}
	Texture2D(std::string filename_);
	~Texture2D();

	std::string GetFilename() { return filename; }
	ID3D11Resource* GetTextureData() { return data; }
	ID3D11ShaderResourceView* GetSRV() { return srv; }
	void SetWidth(uint32_t width_) { width_ = width; }
	void SetHeight(uint32_t height_) { height_ = height; }
	uint32_t GetWidth() { return width; }
	uint32_t GetHeight() { return height; }
};
