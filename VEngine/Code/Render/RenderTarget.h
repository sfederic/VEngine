#pragma once

#include <d3d11.h>

class RenderTarget
{
public:
	RenderTarget(DXGI_FORMAT format_) : format(format_) {}

	void Create(uint32_t width, uint32_t height);
	void Recycle();

	auto& GetRTV() { return *rtv; }
	auto GetRTVAddress() { return &rtv; }

	auto& GetSRV() { return *srv; }
	auto GetSRVAddress() { return &srv; }

private:
	ID3D11Texture2D* texture = nullptr;
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;

	DXGI_FORMAT format;
};
