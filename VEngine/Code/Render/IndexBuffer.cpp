#include "vpch.h"
#include "IndexBuffer.h"
#include "Render/RenderUtils.h"

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
