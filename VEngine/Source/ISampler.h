#pragma once

struct ID3D11SamplerState;


//Shader sampler state for rendering APIs
class ISampler
{
public:
	virtual void Create() = 0;
	virtual void* Get() = 0;

	void* data;
};

class D3D11Sampler : public ISampler
{
public:
	D3D11Sampler();
	virtual void Create();
	virtual void* Get();

	//ID3D11SamplerState* data;
};

class D3D12Sampler : public ISampler
{
public:
	D3D12Sampler();
	virtual void Create();
	virtual void* Get();
};