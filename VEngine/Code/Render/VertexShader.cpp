#include "vpch.h"
#include "VertexShader.h"
#include "Renderer.h"
#include "Core/Debug.h"

void VertexShader::Create(const std::wstring filename)
{
	ReadData(filename);

	HR(Renderer::GetDevice().CreateVertexShader(
		GetByteCodeData(),
		GetByteCodeSize(),
		nullptr,
		GetShaderAddress()));
}

void VertexShader::Reset()
{
	byteCode.clear();
}
