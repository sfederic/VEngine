export module Render.RenderTarget;

import <cstdint>;

#include <d3d11.h>
#include <wrl.h>

export class RenderTarget
{
public:
	void Create(uint32_t width, uint32_t height, DXGI_FORMAT format);
	void CreateFromSwapchainBackBuffer(ID3D11Texture2D* backBuffer);
	void Recycle();

	auto& GetRTV() { return *rtv.Get(); }
	auto GetRTVAddress() { return rtv.GetAddressOf(); }

	auto& GetSRV() { return *srv.Get(); }
	auto GetSRVAddress() { return srv.GetAddressOf(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
};
