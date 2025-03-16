#include <d3d11.h>

import Render.DepthStencil;
import Core.Debug;
import Render.Renderer;

void DepthStencil::Create(int width, int height, const DXGI_SAMPLE_DESC& sampleDesc)
{
	D3D11_TEXTURE2D_DESC dsDesc = {};
	dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsDesc.ArraySize = 1;
	dsDesc.MipLevels = 1;
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsDesc.SampleDesc = sampleDesc;
	dsDesc.Width = width;
	dsDesc.Height = height;

	HR(Renderer::Get().GetDevice().CreateTexture2D(&dsDesc, nullptr, _depthStencilBuffer.GetAddressOf()));

	HR(Renderer::Get().GetDevice().CreateDepthStencilView(_depthStencilBuffer.Get(),
		nullptr, _depthStencilView.GetAddressOf()));
}

void DepthStencil::Reset()
{
	_depthStencilView.Reset();
}

void DepthStencil::ClearView()
{
	Renderer::Get().GetDeviceContext().ClearDepthStencilView(
		_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
