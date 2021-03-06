#pragma once

#include <stdint.h>

struct ID3D11Buffer;

//Buffer interface for the rendering APIs
class Buffer
{
public:
	void Create();

	ID3D11Buffer* data;
	uint64_t size;
};
