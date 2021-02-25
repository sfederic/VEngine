#pragma once

struct ID3D11ShaderResourceView;

//Shader resource view
class IShaderView
{
public:
	virtual void Create() = 0;
	virtual void* Get() = 0;

	void* data;
};

class D3D11ShaderView : public IShaderView
{
public:
	D3D11ShaderView() {}
	D3D11ShaderView(ID3D11ShaderResourceView* srv);
	virtual void Create() override;
	virtual void* Get() override;
};

class D3D12ShaderView : public IShaderView
{
public:
	virtual void Create() override;
	virtual void* Get() override;
};