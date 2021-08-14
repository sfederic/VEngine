#pragma once
#include <d3dcompiler.h>
#include <wrl.h>

using namespace Microsoft::WRL;

struct ShaderSystem
{
	ComPtr<ID3DBlob> CreateShaderFromFile(const wchar_t* filename, const char* entry, const char* target);
};

extern ShaderSystem shaderSystem;
