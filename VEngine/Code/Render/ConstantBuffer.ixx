export module Render.ConstantBuffer;

#include <wrl.h>
#include <d3d11.h>

import Render.RenderUtils;

export template <typename T>
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
		HR(RenderUtils::GetDeviceContext().Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
		std::memcpy(mapped.pData, shaderData, sizeof(T));
		RenderUtils::GetDeviceContext().Unmap(buffer.Get(), 0);
	}

	void SetVS()
	{
		RenderUtils::GetDeviceContext().VSSetConstantBuffers(shaderRegister, 1, buffer.GetAddressOf());
	}

	void SetPS()
	{
		RenderUtils::GetDeviceContext().PSSetConstantBuffers(shaderRegister, 1, buffer.GetAddressOf());
	}

	void SetVSAndPS()
	{
		SetVS();
		SetPS();
	}
};
