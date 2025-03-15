export Render.DepthStencil;

#include <d3d11.h>
#include <wrl.h>

export class DepthStencil
{
public:
	void Create(int width, int height, const DXGI_SAMPLE_DESC& sampleDesc);
	void Reset();

	void ClearView();

	auto GetView() { return _depthStencilView.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> _depthStencilBuffer;
};

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
