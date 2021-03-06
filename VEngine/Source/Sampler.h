#pragma once

struct ID3D11SamplerState;

//Shader sampler state for rendering APIs
class Sampler
{
public:
	void Create();
	void Get();

	ID3D11SamplerState* data;
};
