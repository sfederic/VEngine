#pragma once

struct ID3D11ShaderResourceView;

//Shader resource view
class IShaderView
{
public:
	virtual void Create() = 0;
	virtual void* Get() { return data; }

	void* data;
};

class D3D11ShaderView : public IShaderView
{
public:
	D3D11ShaderView() {}
	D3D11ShaderView(ID3D11ShaderResourceView* srv);
	virtual void Create() override;
};

class D3D12ShaderView : public IShaderView
{
public:
	virtual void Create() override;
};