#include "vpch.h"
#include "PipelineObjects.h"

RastState::RastState(std::string name_, ID3D11RasterizerState* data_)
{
	name = name_;
	data = data_;
}

RastState::~RastState()
{
	if (data)
	{
		data->Release();
	}
}

Sampler::Sampler(ID3D11SamplerState* data_)
{
	data = data_;
}

Sampler::~Sampler()
{
	if (data)
	{
		data->Release();
	}
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
	if (data)
	{
		data->Release();
	}
}

Buffer::~Buffer()
{
	if (data)
	{
		data->Release();
	}
}

ShaderResourceView::~ShaderResourceView()
{
	if (data)
	{
		data->Release();
	}
}

void MeshBuffers::Destroy()
{
	delete vertexBuffer;
	delete indexBuffer;
}
