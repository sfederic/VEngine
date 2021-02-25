#include "ITexture.h"

D3D11Texture::D3D11Texture(ID3D11Texture* texture)
{
	data = texture;
}

D3D11Texture::D3D11Texture(ID3D11Resource* texture)
{
	data = texture;
}

void D3D11Texture::Create()
{
}

D3D12Texture::D3D12Texture(ID3D12Resource* texture)
{
	data = texture;
}

void D3D12Texture::Create()
{
}
