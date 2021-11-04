#pragma once
#include <string>
#include <d3d11.h>

struct ShaderItem;

struct Buffer
{
	ID3D11Buffer* data = nullptr;
};

struct Texture2D
{
	D3D11_TEXTURE2D_DESC desc = {};
	std::string filename;
	ID3D11Resource* data = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;

	Texture2D() {}
	Texture2D(std::string filename_);
	~Texture2D();
};

struct ShaderResourceView
{
	ID3D11ShaderResourceView* data = nullptr;
};

struct Sampler
{
	D3D11_SAMPLER_DESC desc;
	ID3D11SamplerState* data = nullptr;

	Sampler(D3D11_SAMPLER_DESC desc_, ID3D11SamplerState* data_);
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
	D3D11_BLEND_DESC desc;
	ID3D11BlendState* data = nullptr;
	std::string name;

	BlendState() {}
	BlendState(std::string name_, D3D11_BLEND_DESC desc_, ID3D11BlendState* data_);
};

struct MeshBuffers
{
	Buffer* vertexBuffer = nullptr;
	Buffer* indexBuffer = nullptr;
};

struct PipelineStateObject
{
	Buffer* vertexBuffer = nullptr;
	Buffer* indexBuffer = nullptr;

	PipelineStateObject();
	void Create();
	void SetVertexBuffer(Buffer* vertexBuffer);
	void SetIndexBuffer(Buffer* indexBuffer);
};
