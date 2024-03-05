#pragma once

#include "RenderUtils.h"
#include "Core/Debug.h"
#include <wrl.h>
#include <exception>


struct ID3D11Buffer;

template <typename T>
class ConstantBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	uint32_t shaderRegister = 0;

public:
	void Create(const void* initData, uint32_t shaderRegister_)
	{
		shaderRegister = shaderRegister_;
		RenderUtils::CreateDynamicBuffer(sizeof(T), D3D11_BIND_CONSTANT_BUFFER, initData, buffer);
	}

	void Map(T* shaderData)
	{
		D3D11_MAPPED_SUBRESOURCE mapped = {};
		HR(Renderer::GetDeviceContext().Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
		std::memcpy(mapped.pData, shaderData, sizeof(T));
		Renderer::GetDeviceContext().Unmap(buffer.Get(), 0);
	}

	void SetVS()
	{
		Renderer::GetDeviceContext().VSSetConstantBuffers(shaderRegister, 1, buffer.GetAddressOf());
	}

	void SetPS()
	{
		Renderer::GetDeviceContext().PSSetConstantBuffers(shaderRegister, 1, buffer.GetAddressOf());
	}

	void SetVSAndPS()
	{
		SetVS();
		SetPS();
	}
};
