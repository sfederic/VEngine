#include "PipelineObjects.h"
#include <d3d11.h>

RastState::RastState(std::string name_, ID3D11RasterizerState* data_)
{
	name = name_;
	data = data_;
}

Sampler::Sampler(ID3D11SamplerState* data_)
{
	data = data_;
}

Texture2D::Texture2D(std::string filename_)
{
	filename = filename_;
}

Texture2D::~Texture2D()
{
	data->Release();
	srv->Release();
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
