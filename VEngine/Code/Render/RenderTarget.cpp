#include "vpch.h"
#include "RenderTarget.h"
#include "Render/RenderUtils.h"
#include "Core/Debug.h"
#include "Renderer.h"

void RenderTarget::Create(uint32_t width, uint32_t height)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Format = format;
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.SampleDesc.Count = 1;
	desc.Width = width;
	desc.Height = height;

	HR(Renderer::GetDevice().CreateTexture2D(&desc, nullptr, &texture));
	assert(texture);

	HR(Renderer::GetDevice().CreateRenderTargetView(texture.Get(), nullptr, rtv.GetAddressOf()));
	HR(Renderer::GetDevice().CreateShaderResourceView(texture.Get(), nullptr, srv.GetAddressOf()));
}
