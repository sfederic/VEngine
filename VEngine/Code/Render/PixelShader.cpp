#include "vpch.h"

import Core.Debug;

#include "PixelShader.h"
#include "Renderer.h"

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
	shader.Reset();
	byteCode.clear();
}
