#pragma once

#include <stdint.h>

struct ID3D11Buffer;

class Buffer
{
public:

	ID3D11Buffer* data;
	uint64_t size;
};
