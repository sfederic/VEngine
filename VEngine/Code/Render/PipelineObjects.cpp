#include "vpch.h"
#include "PipelineObjects.h"
#include "RenderUtils.h"

RastState::RastState(std::string name_, ID3D11RasterizerState* data_)
{
	name = name_;
	data = data_;
}

BlendState::BlendState(std::string name_, ID3D11BlendState* data_)
{
	name = name_;
	data = data_;
}

void PipelineStateObject::Create(MeshDataProxy& meshDataProxy)
{
	RenderUtils::CreateVertexBuffer(meshDataProxy, vertexBuffer.data);
}

void PipelineStateObject::Destroy()
{
}
