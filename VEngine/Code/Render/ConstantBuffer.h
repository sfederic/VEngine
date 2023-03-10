#pragma once

#include "RenderUtils.h"
#include "Core/Debug.h"

struct ID3D11Buffer;

template <typename T>
class ConstantBuffer
{
	ID3D11Buffer* buffer = nullptr;
	uint32_t shaderRegister = 0;

public:
	ConstantBuffer(const void* initData, uint32_t shaderRegister_) : shaderRegister(shaderRegister_)
	{
		buffer = RenderUtils::CreateDynamicBuffer(sizeof(T), D3D11_BIND_CONSTANT_BUFFER, initData);
	}

	void Map(T* shaderData)
	{
		D3D11_MAPPED_SUBRESOURCE mapped = {};
		HR(RenderUtils::context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
		std::memcpy(mapped.pData, shaderData, sizeof(T));
		RenderUtils::context->Unmap(buffer, 0);
	}

	void SetVS()
	{
		RenderUtils::context->VSSetConstantBuffers(shaderRegister, 1, &buffer);
	}

	void SetPS()
	{
		RenderUtils::context->PSSetConstantBuffers(shaderRegister, 1, &buffer);
	}

	void SetVSAndPS()
	{
		SetVS();
		SetPS();
	}
};
