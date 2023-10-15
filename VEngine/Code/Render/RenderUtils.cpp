#include "vpch.h"
#include "RenderUtils.h"
#include "Texture2D.h"
#include "PipelineObjects.h"
#include "Core/Debug.h"
#include <WICTextureLoader.h>
#include <filesystem>
#include "Core/VString.h"
#include "Asset/AssetPaths.h"
#include "MeshDataProxy.h"
#include "Renderer.h"

namespace RenderUtils
{
	ID3D11Buffer* CreateDefaultBuffer(uint64_t byteWidth, uint32_t bindFlags, const void* initData)
	{
		ID3D11Buffer* buffer = nullptr;

		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = byteWidth;
		desc.BindFlags = bindFlags;
		desc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = initData;

		HR(Renderer::GetDevice().CreateBuffer(&desc, &data, &buffer));

		return buffer;
	}

	//@Todo: there's something up with how D3D11 creates dynamic buffers, where the create will fail sometimes.
	//Hard to find anything on the issue. This stackoverflow issue thinks its this from the official docs:
	//"Create a buffer with D3D11_USAGE_DYNAMIC, and fill it with ID3D11DeviceContext::Map, ID3D11DeviceContext::Unmap (using the Discard and NoOverwrite flags appropriately)."
	//https://stackoverflow.com/questions/25589346/dynamic-vertex-buffer-creation-failure
	//Dynamic buffers also sometimes blow up when map()/unmap()ping the buffer.
	ID3D11Buffer* CreateDynamicBuffer(uint64_t byteWidth, uint32_t bindFlags, const void* initData)
	{
		ID3D11Buffer* buffer;

		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = byteWidth;
		desc.BindFlags = bindFlags;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = initData;

		HR(Renderer::GetDevice().CreateBuffer(&desc, &data, &buffer));
		assert(buffer);

		return buffer;
	}

	ID3D11Buffer* CreateVertexBuffer(MeshDataProxy& meshData)
	{
		return CreateDefaultBuffer(meshData.GetVerticesByteWidth(),
			D3D11_BIND_VERTEX_BUFFER, meshData.vertices.data());
	}

	ID3D11ShaderResourceView* CreateSRVForMeshInstance(ID3D11Buffer* structuredBuffer, uint32_t numBufferElements)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.NumElements = numBufferElements;

		ID3D11ShaderResourceView* srv = nullptr;
		HR(Renderer::GetDevice().CreateShaderResourceView(structuredBuffer, &srvDesc, &srv));

		return srv;
	}

	ID3D11Buffer* CreateStructuredBuffer(uint32_t byteWidth, uint32_t byteStride, const void* initData)
	{
		ID3D11Buffer* buffer;

		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = byteWidth;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.StructureByteStride = byteStride;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = initData;

		HR(Renderer::GetDevice().CreateBuffer(&desc, &data, &buffer));

		return buffer;
	}

	Sampler* CreateSampler()
	{
		D3D11_SAMPLER_DESC sampDesc = {};
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

		ID3D11SamplerState* samplerState;
		HR(Renderer::GetDevice().CreateSamplerState(&sampDesc, &samplerState));

		auto sampler = new Sampler(samplerState);
		return sampler;
	}

	void CreateTexture(Texture2D& texture)
	{
		std::wstring path;

		if (texture.GetFilename().empty())
		{
			//Set default texture if filename empty
			texture.SetFilename("test.png");
			path = VString::stows(AssetBaseFolders::texture + texture.GetFilename());
		}
		else
		{
			path = VString::stows(AssetBaseFolders::texture + texture.GetFilename());
		}

		assert(std::filesystem::exists(path) && "Texture file doesn't exist");

		ID3D11Resource* resource = nullptr;
		ID3D11ShaderResourceView* srv = nullptr;
		HR(DirectX::CreateWICTextureFromFile(&Renderer::GetDevice(), path.c_str(), &resource, &srv));
		assert(resource);
		assert(srv);

		texture.SetTextureData(resource);
		texture.SetSRV(srv);
		texture.SetUID(GenerateUID());

		//CreateWICTextureFromFile() doesn't like ID3D11Texture2D, so casting down here
		//to get the texture Desc.
		ID3D11Texture2D* textureResource = nullptr;
		HR(resource->QueryInterface(&textureResource));

		D3D11_TEXTURE2D_DESC texDesc = {};
		textureResource->GetDesc(&texDesc);

		texture.SetWidth(texDesc.Width);
		texture.SetHeight(texDesc.Height);
	}
}
