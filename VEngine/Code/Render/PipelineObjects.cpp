#include "PipelineObjects.h"
#include "RenderUtils.h"
#include "TextureSystem.h"
#include "ShaderSystem.h"
#include "Renderer.h"

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

PipelineStateObject::PipelineStateObject(std::wstring textureFilename_, std::wstring shaderFilename_)
{
	textureFilename = textureFilename_;
	shaderFilename = shaderFilename_;
}

void PipelineStateObject::Create()
{
	texture = textureSystem.FindTexture2D(textureFilename);
	sampler = RenderUtils::GetDefaultSampler();
	shader = shaderSystem.FindShader(shaderFilename);
	rastState = renderer.rastStateMap["solid"];
}

void PipelineStateObject::SetVertexBuffer(Buffer* vertexBuffer_)
{
	vertexBuffer = vertexBuffer_;
}

void PipelineStateObject::SetIndexBuffer(Buffer* indexBuffer_)
{
	indexBuffer = indexBuffer;
}

void PipelineStateObject::SetTexture(Texture2D* texture_)
{
	texture = texture_;
}

void PipelineStateObject::SetSampler(Sampler* sampler_)
{
	sampler = sampler_;
}

void PipelineStateObject::SetRastState(RastState* rastState_)
{
	rastState = rastState;
}

void PipelineStateObject::SetShader(ShaderItem* shaderItem)
{
	shader = shaderItem;
}
