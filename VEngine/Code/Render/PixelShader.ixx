export module Render.PixelShader;

#include <wrl.h>
#include <d3d11.h>

import Render.Shader;
import Core.Debug;

import <string>;

export class PixelShader : public Shader
{
public:
	void Create(const std::wstring filename) override;
	void Reset() override;

	auto GetShader() { return shader.Get(); }
	auto GetShaderAddress() { return shader.GetAddressOf(); }

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
};

void PixelShader::Create(const std::wstring filename)
{
	ReadData(filename);

	HR(Renderer::Get().GetDevice().CreatePixelShader(
		GetByteCodeData(),
		GetByteCodeSize(),
		nullptr,
		GetShaderAddress()));
}

void PixelShader::Reset()
{
	shader.Reset();
	byteCode.clear();
}

