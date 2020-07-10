#pragma once

#include <vector>
#include <map>
#include "DXUtil.h"

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
	ID3DBlob* pixelCode;
};

class ShaderFactory
{
public:
	void CreateAllShaders();
	void CompileAllShadersFromFile();

	std::vector<ShaderItem> shaders;
	std::map<const char*, ShaderItem> shadersMap;
};
