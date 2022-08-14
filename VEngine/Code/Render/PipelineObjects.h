#pragma once
#include <string>

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;
struct ID3D11BlendState;
struct ID3D11RasterizerState;

struct Buffer
{
	ID3D11Buffer* data = nullptr;

	~Buffer();
};

struct ShaderResourceView
{
	ID3D11ShaderResourceView* data = nullptr;

	~ShaderResourceView();
};

struct Sampler
{
	ID3D11SamplerState* data = nullptr;

	Sampler(ID3D11SamplerState* data_);
	~Sampler();
};

struct RastState
{
	ID3D11RasterizerState* data = nullptr;
	std::string name;

	RastState() {}
	RastState(std::string name_, ID3D11RasterizerState* data_);
	~RastState();
};

struct BlendState
{
	ID3D11BlendState* data = nullptr;
	std::string name;

	BlendState() {}
	BlendState(std::string name_, ID3D11BlendState* data_);
	~BlendState();
};

struct MeshBuffers
{
	Buffer vertexBuffer;
	Buffer indexBuffer;

	void Destroy();
};

struct PipelineStateObject
{
	Buffer* vertexBuffer = nullptr;
	Buffer* indexBuffer = nullptr;

	void SetVertexBuffer(Buffer* vertexBuffer);
	void SetIndexBuffer(Buffer* indexBuffer);
};
