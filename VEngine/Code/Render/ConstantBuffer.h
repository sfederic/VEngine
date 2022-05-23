#pragma once
#include "RenderUtils.h"

struct ID3D11Buffer;

template <typename T>
class ConstantBuffer
{
	ID3D11Buffer* data = nullptr;

public:
	ConstantBuffer(const void* initData)
	{
		data = RenderUtils::CreateDynamicBuffer(sizeof(T), D3D11_BIND_CONSTANT_BUFFER, initData);
		assert(data);
	}

	ID3D11Buffer* GetBuffer() { return data; }
};
