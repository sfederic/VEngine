#pragma once

#include <string>

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;
struct ID3D11BlendState;
struct ID3D11RasterizerState;

struct MeshDataProxy;

struct Buffer
{
	ID3D11Buffer* data = nullptr;
};

struct ShaderResourceView
{
	ID3D11ShaderResourceView* data = nullptr;

};

struct Sampler
{
	ID3D11SamplerState* data = nullptr;

	Sampler() {}
	Sampler(ID3D11SamplerState* data_);
};

struct RastState
{
	ID3D11RasterizerState* data = nullptr;
	std::string name;

	RastState() {}
	RastState(std::string name_, ID3D11RasterizerState* data_);
};

struct BlendState
{
	ID3D11BlendState* data = nullptr;
	std::string name;

	BlendState() {}
	BlendState(std::string name_, ID3D11BlendState* data_);
};

struct MeshBuffers
{
	Buffer vertexBuffer;
};

class PipelineStateObject
{
private:
	Buffer vertexBuffer;

public:
	void Create(MeshDataProxy& meshDataProxy);
	void Destroy();

	Buffer& GetVertexBuffer() { return vertexBuffer; }
};
