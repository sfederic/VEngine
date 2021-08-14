#include "ShaderSystem.h"
#include "Debug.h"

ShaderSystem shaderSystem;

ComPtr<ID3DBlob> ShaderSystem::CreateShaderFromFile(const wchar_t* filename, const char* entry, const char* target)
{
	UINT compileFlags = 0;
#ifdef _DEBUG
	compileFlags = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#endif
	ComPtr<ID3DBlob> code;
	ComPtr<ID3DBlob> error;

	HR(D3DCompileFromFile(filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry, target, compileFlags, 0, code.GetAddressOf(), error.GetAddressOf()));

	if (error)
	{
		const wchar_t* errMsg = (wchar_t*)error->GetBufferPointer();
	}

	return code;
}