#pragma once
class IRasterizerState
{
public:
	virtual void Create() = 0;
	virtual void* Get() = 0;

	void* data;
};

class D3D11RasterizerState : public IRasterizerState
{
public:
	virtual void Create();
	virtual void* Get();
};

class D3D12RasterizerState : public IRasterizerState
{
public:
	virtual void Create();
	virtual void* Get();
};