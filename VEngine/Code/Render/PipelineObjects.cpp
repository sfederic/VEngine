#include "PipelineObjects.h"

RastState::RastState(std::string name_, D3D11_RASTERIZER_DESC desc_, ID3D11RasterizerState* data_)
{
	name = name_;
	desc = desc_;
	data = data_;
}

Sampler::Sampler(D3D11_SAMPLER_DESC desc_, ID3D11SamplerState* data_)
{
	desc = desc_;
	data = data_;
}

Texture2D::Texture2D(std::wstring filename_)
{
	filename = filename_;
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
