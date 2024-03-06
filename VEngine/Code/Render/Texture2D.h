#pragma once

#include <string>
#include <Core/UID.h>
#include <wrl.h>

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

class Texture2D
{
public:
	Texture2D() {}
	Texture2D(std::string filename_) : filename(filename_) {}
	~Texture2D();

	void Create();

	auto GetFilename() { return filename; }

	auto GetTextureData() { return data.Get(); }
	auto GetSRV() { return srv.Get(); }

	auto GetWidth() const { return width; }
	auto GetHeight() const { return height; }

	auto GetUID() const { return uid; }

	void SetBufferNames();

private:
	std::string filename;

	Microsoft::WRL::ComPtr<ID3D11Resource> data;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;

	UID uid = 0;

	uint32_t width = 0;
	uint32_t height = 0;
};
