#pragma once

struct Buffer
{
	struct ID3D11Buffer* data;
};

struct Texture2D
{
	struct ID3D11Texture2D* data;
};

struct ShaderResourceView
{
	struct ID3D11ShaderResourceView* data;
};

struct Sampler
{
	struct ID3D11SamplerState* data;
};

struct RasterizerState
{
	struct ID3D11RasterizerState* data;
};

struct BlendState
{
	struct ID3D11BlendState* data;
};

struct PipelineStateObject
{
	Buffer vertexBuffer;
	Buffer indexBuffer;
	Sampler sampler;
};
