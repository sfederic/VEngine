#include <d3d11.h>
#include <wrl.h>

export module Render.VertexBuffer;

import Render.Vertex;
import Render.RenderUtils;
import <vector>;

struct MeshDataProxy;

export class VertexBuffer
{
public:
	void CreateDefault(MeshDataProxy& meshDataProxy);
	void CreateDynamic(std::vector<Vertex>& vertices);
	void CreateDynamicCapped(std::vector<Vertex>& vertexData, uint32_t cappedSize);
	void Destroy();

	auto GetDataAddress() { return data.GetAddressOf(); }
	auto GetData() { return data.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> data;
};

void VertexBuffer::CreateDefault(MeshDataProxy& meshDataProxy)
{
	RenderUtils::CreateVertexBuffer(meshDataProxy, data);
}

void VertexBuffer::CreateDynamic(std::vector<Vertex>& vertices)
{
	RenderUtils::CreateDynamicBuffer(sizeof(Vertex) * vertices.size(),
		D3D11_BIND_VERTEX_BUFFER, vertices.data(), data);
}

void VertexBuffer::CreateDynamicCapped(std::vector<Vertex>& vertexData, uint32_t cappedSize)
{
	RenderUtils::CreateDynamicBuffer(sizeof(Vertex) * cappedSize,
		D3D11_BIND_VERTEX_BUFFER, vertexData.data(), data);
}

void VertexBuffer::Destroy()
{
	data.Reset();
}
