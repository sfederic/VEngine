#pragma once

#include <dxgi1_6.h>
#include <d3d11.h>
#include <wrl.h>

class Swapchain
{
public:
	inline static constexpr int SWAPCHAIN_COUNT = 2;

	void Create(int width,
		int height,
		HWND window,
		const DXGI_SAMPLE_DESC& sampleDesc,
		ID3D11Device* device,
		IDXGIFactory* dxgiFactory);

	void Reset();

	auto GetCurrentBackBufferIndex() { return _swapchain.Get()->GetCurrentBackBufferIndex(); }
	void ResizeBuffers(int newWidth, int newHeight);

	void Present();

	auto Get() { return _swapchain.Get(); }
	void GetBackBuffer(ID3D11Texture2D** backBuffer);

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain3> _swapchain;
};
