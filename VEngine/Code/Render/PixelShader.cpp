#include "vpch.h"
#include "PixelShader.h"
#include "RenderUtils.h"
#include "Core/Debug.h"

void PixelShader::Create(const std::wstring filename)
{
	hlslFilepath = filename;
	ReadData(filename);

	HR(RenderUtils::device->CreatePixelShader(
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
