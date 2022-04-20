#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <d3dcompiler.h>
#include <wrl.h>
#include "System.h"

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11ComputeShader;

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

	ID3DBlob* vertexCode = nullptr;
	ID3DBlob* pixelCode = nullptr;
	ID3DBlob* computeCode = nullptr;

	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11ComputeShader* computeShader = nullptr;
};

//@Todo: need to make a CompileShaderFromFile() function eventually to work with compiled files over text

struct ShaderSystem : System
{
private:
	std::vector<ShaderItem*> shaders;
	std::vector<ShaderItem*> computeShaders;
	std::unordered_map<std::wstring, ShaderItem*> shaderMap;

public:
	ShaderSystem();
	void Init();
	void Tick();
	ShaderItem* FindShader(std::wstring shaderName);
	void CleanUpShaders();

private:
	ID3DBlob* CreateShaderFromFile(const wchar_t* filename, const char* entry, const char* target);
	void CreateAllShaders();
	void CompileAllShadersFromFile();
	void HotReloadShaders();
};

extern ShaderSystem shaderSystem;
