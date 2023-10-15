#include "vpch.h"
#include "VertexShader.h"
#include "RenderUtils.h"
#include "Core/Debug.h"

void VertexShader::Create(const std::wstring filename)
{
	ReadData(filename);

	HR(RenderUtils::device->CreateVertexShader(
		GetByteCodeData(),
		GetByteCodeSize(),
		nullptr,
		GetShaderAddress()));
}

void VertexShader::Reset()
{
	shader.Reset();
	byteCode.clear();
}
