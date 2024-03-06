#include "vpch.h"
#include "Texture2D.h"
#include "Asset/AssetPaths.h"
#include "Core/Debug.h"
#include "Render/RenderUtils.h"
#include "Render/Renderer.h"
#include <WICTextureLoader.h>
#include <cassert>
#include <filesystem>

Texture2D::~Texture2D()
{
	width = 0;
	height = 0;
}

void Texture2D::Create()
{
	std::wstring path;

	if (GetFilename().empty())
	{
		//Set default texture if filename empty
		filename = "test.png";
		path = VString::stows(AssetBaseFolders::texture + GetFilename());
	}
	else
	{
		path = VString::stows(AssetBaseFolders::texture + GetFilename());
	}

	assert(std::filesystem::exists(path) && "Texture file doesn't exist");

	uid = GenerateUID();

	//SRBG
	//HR(DirectX::CreateWICTextureFromFileEx(&Renderer::GetDevice(), &Renderer::GetDeviceContext(),
	//	path.c_str(),
	//	0,
	//	D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
	//	WIC_LOADER_FORCE_SRGB,
	//	&resource, &srv));

	HR(DirectX::CreateWICTextureFromFile(&Renderer::GetDevice(), path.c_str(), data.GetAddressOf(), srv.GetAddressOf()));

	//CreateWICTextureFromFile() doesn't like ID3D11Texture2D, so casting down here to get the texture desc.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> textureResource;
	data.As(&textureResource);

	D3D11_TEXTURE2D_DESC texDesc = {};
	textureResource->GetDesc(&texDesc);

	width = texDesc.Width;
	height = texDesc.Height;

	SetBufferNames();
}

void Texture2D::SetBufferNames()
{
	RenderUtils::SetResourceName(data.Get(), "Resource_" + filename);
	RenderUtils::SetResourceName(srv.Get(), "SRV_" + filename);
}
