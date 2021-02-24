#pragma once
class IRasterizerState
{
public:
	virtual void Create() = 0;
};

class D3D11RasterizerState : public IRasterizerState
{
public:
	virtual void Create();
};

class D3D12RasterizerState : public IRasterizerState
{
public:
	virtual void Create();
};