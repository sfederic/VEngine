#include "vpch.h"
#include "PipelineObjects.h"

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

PipelineStateObject::PipelineStateObject()
{
}

void PipelineStateObject::Create()
{
}

void PipelineStateObject::SetVertexBuffer(Buffer* vertexBuffer_)
{
	vertexBuffer = vertexBuffer_;
}

void PipelineStateObject::SetIndexBuffer(Buffer* indexBuffer_)
{
	indexBuffer = indexBuffer;
}

BlendState::BlendState(std::string name_, ID3D11BlendState* data_)
{
	name = name_;
	data = data_;
}

BlendState::~BlendState()
{
	data->Release();
}

Buffer::~Buffer()
{
	data->Release();
}

ShaderResourceView::~ShaderResourceView()
{
	data->Release();
}

MeshBuffers::~MeshBuffers()
{
	delete vertexBuffer;
	delete indexBuffer;
}
