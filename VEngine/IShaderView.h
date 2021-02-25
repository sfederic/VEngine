#pragma once

//Shader resource view
class IShaderView
{
public:
	virtual void Create() = 0;
	virtual void* Get() = 0;
};

class D3D11ShaderView : public IShaderView
{
public:
	virtual void Create() override;
	virtual void* Get() override;
};

class D3D12ShaderView : public IShaderView
{
public:
	virtual void Create() override;
	virtual void* Get() override;
};