#pragma once

#include <string>
#include <Core/UID.h>

class VertexShader;
class PixelShader;
struct ID3D11VertexShader;
struct ID3D11PixelShader;

//Helper structure when finding shaders from Shader System
class ShaderItem
{
public:
	ShaderItem(std::string shaderItemName_,
		std::wstring vertexShaderFilename_,
		std::wstring pixelShaderFilename_);

	auto GetVertexShader() { return vertexShader; }
	auto GetPixelShader() { return pixelShader; }

	auto GetName() const { return shaderItemName; }

	auto GetVertexShaderFilename() { return vertexShaderFilename; }
	auto GetPixelShaderFilename() { return pixelShaderFilename; }

	auto GetUID() const { return uid; }

private:
	//UID here is used for sorting meshes on render.
	UID uid = GenerateUID();

	std::string shaderItemName;

	std::wstring vertexShaderFilename;
	std::wstring pixelShaderFilename;

	//Caches of the shaders. These pointers aren't created or released directly, hence not being ComPtrs.
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
};
