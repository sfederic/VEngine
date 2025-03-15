export module Render.Device;

#include <d3d11.h>
#include <wrl.h>

import Core.Debug;

export class Device
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

void Device::Create()
{
	//BGRA support needed for DirectWrite and Direct2D
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };
	D3D_FEATURE_LEVEL selectedFeatureLevel;

	//@Todo: this shit always causes problems. From HDR to graphics debugging, all sorts of issues.
	//For now, keep adapter CreateDevice() input as nullptr. Change on release.
	//IDXGIAdapter1* adapter = nullptr;
	//HR(dxgiFactory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)));

	HR(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
		featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, device.GetAddressOf(),
		&selectedFeatureLevel, deviceContext.GetAddressOf()));

	//Create debug device
	HR(device->QueryInterface(IID_PPV_ARGS(debugDevice.GetAddressOf())));
}
