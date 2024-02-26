#pragma once

#include <string>
#include <Core/UID.h>

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

//@Todo: Removed destructor over render multi thread frame packet.
//Needs a cleanup function to call from TextureSystem.
class Texture2D
{
public:
	Texture2D() {}
	Texture2D(std::string filename_) : filename(filename_) {}

	std::string GetFilename() { return filename; }
	void SetFilename(std::string filename_) { filename = filename_; }

	void SetTextureData(ID3D11Resource* data_) { data = data_; }
	ID3D11Resource* GetTextureData() { return data; }

	void SetSRV(ID3D11ShaderResourceView* srv_) { srv = srv_; }
	ID3D11ShaderResourceView* GetSRV() { return srv; }

	void SetWidth(uint32_t width_) { width = width_; }
	void SetHeight(uint32_t height_) { height = height_; }
	uint32_t GetWidth() const { return width; }
	uint32_t GetHeight() const { return height; }

	UID GetUID() const { return uid; }
	void SetUID(UID uid_) { uid = uid_; }

private:
	UID uid = 0;

	std::string filename;

	ID3D11Resource* data = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;

	uint32_t width = 0;
	uint32_t height = 0;
};
