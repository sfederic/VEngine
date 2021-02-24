#pragma once

//Shader resource view
class IShaderView
{
public:
	virtual void Create() = 0;
};

class D3D11ShaderView : public IShaderView
{
public:
	virtual void Create() override;
};

class D3D12ShaderView : public IShaderView
{
public:
	virtual void Create() override;
};