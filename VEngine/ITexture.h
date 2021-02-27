#pragma once

struct ID3D11Texture;
struct ID3D11Resource;
struct ID3D12Resource;

//D3D has the tendency to call Textures 'Resources'
class ITexture
{
public:
	virtual void Create() = 0;
	void* Get();
	void* data;
};

class D3D11Texture : public ITexture
{
public:
	D3D11Texture() {}
	D3D11Texture(ID3D11Texture* texture);
	D3D11Texture(ID3D11Resource* texture);
	virtual void Create();
};

class D3D12Texture : public ITexture
{
public:
	D3D12Texture() {}
	D3D12Texture(ID3D12Resource* texture);
	virtual void Create();
};