#pragma once

//Buffer interface for the rendering APIs
class IBuffer
{
public:
	virtual void Create() = 0;
};

class D3D12Buffer : IBuffer
{
public:
	virtual void Create();
};

class D3D11Buffer : IBuffer
{
public:
	virtual void Create();
};