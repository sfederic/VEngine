#pragma once

#include <stdint.h>

struct ID3D11Buffer;
struct ID3D12Resource;

//Buffer interface for the rendering APIs
class IBuffer
{
public:
	virtual void Create() = 0;
	virtual void* Get() = 0; //Returns buffer data

	void* data;
	uint64_t size;
};

class D3D11Buffer : public IBuffer
{
public:
	virtual void Create();
	virtual void* Get() override;
};

class D3D12Buffer : public IBuffer
{
public:
	virtual void Create();
	virtual void* Get();
};
