#pragma once

#include <string>
#include <wrl.h>

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;
struct ID3D11BlendState;
struct ID3D11RasterizerState;

struct MeshDataProxy;

struct Buffer
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> data;
};

struct ShaderResourceView
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> data;
};

struct Sampler
{
	Microsoft::WRL::ComPtr<ID3D11SamplerState> data;
};

struct RastState
{
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> data;
	std::string name;

	RastState(std::string name_, ID3D11RasterizerState* data_);
};

struct BlendState
{
	Microsoft::WRL::ComPtr<ID3D11BlendState> data;
	std::string name;

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
