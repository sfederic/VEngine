#pragma once

//Shader sampler state for rendering APIs
class ISampler
{
public:
	virtual void Create() = 0;
};

class D3D11Sampler : public ISampler
{
public:
	virtual void Create();
};

class D3D12Sampler : public ISampler
{
public:
	virtual void Create();
};