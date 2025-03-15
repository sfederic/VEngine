export module Render.INdexBuffer;

#include <d3d11.h>
#include <wrl.h>

import <vector>;

import Render.RenderUtils;
import Render.MeshData;

export class IndexBuffer
{
public:
	void CreateDefault(std::vector<MeshData::indexDataType>& indices);
	void CreateDynamic(std::vector<MeshData::indexDataType>& indices);
	void CreateDynamicCapped(std::vector<MeshData::indexDataType>& indexData, uint32_t cappedSize);
	void Destroy();

	auto GetDataAddress() { return data.GetAddressOf(); }
	auto GetData() { return data.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> data;
};

void IndexBuffer::CreateDefault(std::vector<MeshData::indexDataType>& indices)
{
	RenderUtils::CreateIndexBuffer(indices, data);
}

void IndexBuffer::CreateDynamic(std::vector<MeshData::indexDataType>& indices)
{
	RenderUtils::CreateDynamicBuffer(sizeof(MeshData::indexDataType) * indices.size(),
		D3D11_BIND_INDEX_BUFFER, indices.data(), data);
}

void IndexBuffer::CreateDynamicCapped(std::vector<MeshData::indexDataType>& indexData, uint32_t cappedSize)
{
	RenderUtils::CreateDynamicBuffer(sizeof(MeshData::indexDataType) * cappedSize,
		D3D11_BIND_INDEX_BUFFER, indexData.data(), data);
}

void IndexBuffer::Destroy()
{
	data.Reset();
}
