#pragma once

#include <vector>
#include <unordered_map>

//NOTE: All the stuff in these files assumes using joined shaders for now (eg. vert/pixel in same file)

enum class EShaderType
{
	VERTEX,
	PIXEL,
	GEOMETRY,
	COMPUTE,
	HULL,
	DOMAIN_SHADER, //DOMAIN declared in corecrt_math.h
	UNKNOWN //For testing (creating 'empty' shaders)
};

enum class EShaderID
{
	DEBUG_DRAW,
	SHADERS
};

struct ShaderItem
{
	ShaderItem() {};
	
	ShaderItem(const wchar_t* filenameInit, EShaderType typeInit)
	{
		wcscpy_s(filename, 64 * 2, filenameInit);
		type = typeInit;
	}

	wchar_t filename[64];
	EShaderType type;

	struct ID3DBlob* vertexCode; 
	struct ID3DBlob* pixelCode; 

	struct ID3D11VertexShader* vertexShader;
	struct ID3D11PixelShader* pixelShader;
};

class ShaderFactory
{
public:
	void CreateAllShaders(struct ID3D11Device* device);
	void CompileAllShadersFromFile();
	void InitHotLoading();
	void CleanUpShaders();
	void HotReloadShaders();

	std::unordered_map<std::wstring, ShaderItem> shaders;

	HANDLE hotreloadHandle;
};
