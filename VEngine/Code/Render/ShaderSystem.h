#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <d3dcompiler.h>
#include <wrl.h>
#include "System.h"

using namespace Microsoft::WRL;

struct ShaderItem
{
	ShaderItem()
	{
	}

	ShaderItem(const wchar_t* _filename)
	{
		filename = _filename;
	}

	std::wstring filename;

	ID3DBlob* vertexCode;
	ID3DBlob* pixelCode;

	struct ID3D11VertexShader* vertexShader;
	struct ID3D11PixelShader* pixelShader;
};

//@Todo: need to make a CompileShaderFromFile() function eventually to work with compiled files over text

struct ShaderSystem : System
{
private:
	std::vector<ShaderItem> shaders;
	std::unordered_map<std::wstring, ShaderItem*> shaderMap;

public:
	ShaderSystem();
	void Init();
	void Tick();
	ShaderItem* FindShader(std::wstring shaderName);
	ID3DBlob* CreateShaderFromFile(const wchar_t* filename, const char* entry, const char* target);
	void CreateAllShaders();
	void CompileAllShadersFromFile();
	void CleanUpShaders();
	void InitHotLoading();
	void HotReloadShaders();
};

extern ShaderSystem shaderSystem;
