#pragma once

#include <d3d11.h>
#include <wrl.h>

class Device
{
public:
	void Create();

	auto Get() { return device.Get(); }
	auto GetContext() { return deviceContext.Get(); }
	auto GetDebugDevice() { return debugDevice.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11Debug> debugDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
};
