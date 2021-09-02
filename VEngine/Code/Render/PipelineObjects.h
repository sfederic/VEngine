#pragma once

struct ID3D11Buffer;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;
struct ID3D11RasterizerState;
struct ID3D11BlendState;

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

struct RasterizerState
{
	ID3D11RasterizerState* data = nullptr;
};

struct BlendState
{
	ID3D11BlendState* data = nullptr;
};

struct PipelineStateObject
{
	PipelineStateObject();

	Buffer vertexBuffer;
	Buffer indexBuffer;
	Sampler sampler;
	RasterizerState rastState;
};
