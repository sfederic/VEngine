#include "vpch.h"
#include "PixelShader.h"
#include "Renderer.h"
#include "Core/Debug.h"

void PixelShader::Create(const std::wstring filename)
{
	hlslFilepath = filename;
	ReadData(filename);

	HR(Renderer::GetDevice().CreatePixelShader(
		GetByteCodeData(),
		GetByteCodeSize(),
		nullptr,
		GetShaderAddress()));
}

void PixelShader::Reset()
{
	byteCode.clear();
}
