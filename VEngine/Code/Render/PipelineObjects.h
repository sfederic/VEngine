#pragma once
#include <string>
#include <d3d11.h>


struct Buffer
{
	ID3D11Buffer* data = nullptr;
};

struct Texture2D
{
	ID3D11Texture2D* data = nullptr;
};

struct ShaderResourceView
{
	ID3D11ShaderResourceView* data = nullptr;
};

struct Sampler
{
	ID3D11SamplerState* data = nullptr;
};

struct RastState
{
	D3D11_RASTERIZER_DESC desc;
	ID3D11RasterizerState* data = nullptr;
	std::string name;

	RastState() {}
	RastState(std::string name_, D3D11_RASTERIZER_DESC desc_, ID3D11RasterizerState* data_);
};

struct BlendState
{
	ID3D11BlendState* data = nullptr;
};

struct PipelineStateObject
{
	PipelineStateObject() {}

	Buffer vertexBuffer;
	Buffer indexBuffer;
	Sampler sampler;
	RastState* rastState;
};
