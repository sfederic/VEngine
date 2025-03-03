#include "vpch.h"
#include "Swapchain.h"
#include <d3d11.h>
#include "Core/Debug.h"

void Swapchain::Create(int width,
	int height,
	HWND window,
	const DXGI_SAMPLE_DESC& sampleDesc,
	ID3D11Device* d3dDevice,
	IDXGIFactory* dxgiFactory)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc = { (UINT)width, (UINT)height, {60, 1}, DXGI_FORMAT_R8G8B8A8_UNORM };
	sd.Windowed = TRUE;
	sd.SampleDesc = sampleDesc;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = window;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.BufferCount = SWAPCHAIN_COUNT;

	IDXGISwapChain* tempSwapchain = nullptr;
	HR(dxgiFactory->CreateSwapChain(d3dDevice, &sd, &tempSwapchain));
	HR(tempSwapchain->QueryInterface(_swapchain.GetAddressOf()));
	tempSwapchain->Release();

	HR(dxgiFactory->MakeWindowAssociation(window, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER));
}

void Swapchain::Reset()
{
	_swapchain.Reset();
}

void Swapchain::ResizeBuffers(int newWidth, int newHeight)
{
	HR(_swapchain->ResizeBuffers(SWAPCHAIN_COUNT, newWidth, newHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
}

void Swapchain::Present()
{
	HR(_swapchain->Present(1, 0));
}

void Swapchain::GetBackBuffer(ID3D11Texture2D** backBuffer)
{
	HR(_swapchain->GetBuffer(0, IID_PPV_ARGS(backBuffer)));
}
