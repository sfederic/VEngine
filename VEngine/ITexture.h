#pragma once

//D3D has the tendency to call Textures 'Resources'
class ITexture
{
public:
	virtual void Create() = 0;
};

class D3D11Texture : public ITexture
{
public:
	virtual void Create();
};

class D3D12Texture : public ITexture
{
public:
	virtual void Create();
};