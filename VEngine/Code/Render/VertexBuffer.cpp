#include "vpch.h"
#include "VertexBuffer.h"
#include "Render/RenderUtils.h"

void VertexBuffer::CreateDefault(MeshDataProxy& meshDataProxy)
{
	RenderUtils::CreateVertexBuffer(meshDataProxy, data);
}

void VertexBuffer::CreateDynamic(std::vector<Vertex>& vertices)
{
	RenderUtils::CreateDynamicBuffer(sizeof(Vertex) * vertices.size(),
		D3D11_BIND_VERTEX_BUFFER, vertices.data(), data);
}

void VertexBuffer::Destroy()
{
	data.Reset();
}
