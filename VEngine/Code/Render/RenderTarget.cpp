import Render.RenderTarget;
import Core.Debug;
import <cassert>;

void RenderTarget::Create(uint32_t width, uint32_t height, DXGI_FORMAT format)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Format = format;
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.SampleDesc.Count = 1;
	desc.Width = width;
	desc.Height = height;

	//RenderTarget is using a D3D11 texture instead of the engine texture because Texture2D doesn't have 
	//anything to pass in a D3D11_TEXTURE2D_DESC to the DXToolkit functions.
	HR(Renderer::Get().GetDevice().CreateTexture2D(&desc, nullptr, &texture));
	assert(texture);

	HR(Renderer::Get().GetDevice().CreateRenderTargetView(texture.Get(), nullptr, rtv.GetAddressOf()));
	HR(Renderer::Get().GetDevice().CreateShaderResourceView(texture.Get(), nullptr, srv.GetAddressOf()));
}

void RenderTarget::CreateFromSwapchainBackBuffer(ID3D11Texture2D* backBuffer)
{
	HR(Renderer::Get().GetDevice().CreateRenderTargetView(backBuffer, nullptr, rtv.GetAddressOf()));
}

void RenderTarget::Recycle()
{
	texture.Reset();
	rtv.Reset();
	srv.Reset();
}
