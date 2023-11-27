#include "vpch.h"
#include "PipelineObjects.h"
#include "RenderUtils.h"

RastState::RastState(std::string name_, ID3D11RasterizerState* data_)
{
	name = name_;
	data = data_;
}

RastState::~RastState()
{
	data->Release();
}

Sampler::Sampler(ID3D11SamplerState* data_)
{
	data = data_;
}

Sampler::~Sampler()
{
	data->Release();
}

BlendState::BlendState(std::string name_, ID3D11BlendState* data_)
{
	name = name_;
	data = data_;
}

BlendState::~BlendState()
{
	if (data)
	{
		data->Release();
	}
}

ShaderResourceView::~ShaderResourceView()
{
	data->Release();
}

void PipelineStateObject::Create(MeshDataProxy& meshDataProxy)
{
	vertexBuffer.data = RenderUtils::CreateVertexBuffer(meshDataProxy);
}

void PipelineStateObject::Destroy()
{
	vertexBuffer.data->Release();
}
