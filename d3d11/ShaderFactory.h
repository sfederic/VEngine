#pragma once

#include <vector>
#include <unordered_map>
#include "RenderSystem.h"

//NOTE: All the stuff in these files assumes using joined shaders

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
	
	ShaderItem(const wchar_t* _filename, EShaderType _type)
	{
		wcscpy_s(filename, 64 * 2, _filename);
		type = _type;
	}

	wchar_t filename[64];
	EShaderType type;

	ID3DBlob* vertexCode; //Can you do away with the interfaces and store the blobs contiguously?
	ID3DBlob* pixelCode; //TODO: make sure to free() after future work with InputLayout

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
};

class ShaderFactory
{
public:
	void CreateAllShaders(ID3D11Device* device);
	void CompileAllShadersFromFile();
	void InitHotLoading();
	void CleanUpShaders();
	void HotReloadShaders(ID3D11Device* device, class DebugMenu* debugMenu);

	std::vector<ShaderItem> shaders;
	std::unordered_map<std::wstring, ShaderItem*> shadersMap; //I'm watching you std::wstring. Always watching

	HANDLE hotreloadHandle;
};
