#include <d3d11.h>
#include <wrl.h>

export module Render.DepthStencil;

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
