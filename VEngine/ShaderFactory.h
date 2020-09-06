#pragma once

#include "RenderSystem.h"
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
	ShaderItem() {}
	ShaderItem(const wchar_t* filenameInit, EShaderType typeInit)
	{
		wcscpy_s(filename, 64 * 2, filenameInit);
		type = typeInit;
	}

	wchar_t filename[64];
	EShaderType type;

	ID3DBlob* vertexCode; 
	ID3DBlob* pixelCode; 

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11ShaderResourceView* srv;
	ID3D11Texture2D* texture;
};

class ShaderFactory
{
public:
	void CompileAllShadersFromFile();
	void InitHotLoading();
	void CleanUpShaders();
	void HotReloadShaders();

	std::vector<ShaderItem> shaders;
	std::unordered_map<std::wstring, ShaderItem*> shaderMap;
};

extern ShaderFactory gShaderFactory;
