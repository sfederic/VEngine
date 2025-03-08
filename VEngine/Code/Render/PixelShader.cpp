#include "vpch.h"
#include "PixelShader.h"
#include "Renderer.h"
#include "Core/Debug.h"

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
