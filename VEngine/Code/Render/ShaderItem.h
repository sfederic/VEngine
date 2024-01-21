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

	auto GetName() { return shaderItemName; }

	auto GetVertexShaderFilename() { return vertexShaderFilename; }
	auto GetPixelShaderFilename() { return pixelShaderFilename; }

	auto GetUID() const { return uid; }

private:
	//UID here is used for sorting meshes on render.
	UID uid = GenerateUID();

	std::string shaderItemName;

	std::wstring vertexShaderFilename;
	std::wstring pixelShaderFilename;

	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
};

//@Todo: adding new shaders and shader pairs to the engine is really slow and bad.
//Need some sort of automated visual studio step or an offline compile that iterates through directories
//and builds up a data structure.

struct ShaderItems
{
	inline static ShaderItem* Default;
	inline static ShaderItem* DefaultClip;
	inline static ShaderItem* Unlit;
	inline static ShaderItem* Animation;
	inline static ShaderItem* Shadow;
	inline static ShaderItem* ShadowAnimation;
	inline static ShaderItem* Instance;
	inline static ShaderItem* LightProbe;
	inline static ShaderItem* SolidColour;
	inline static ShaderItem* UI;
	inline static ShaderItem* PostProcess;
	inline static ShaderItem* Water;
	inline static ShaderItem* Floating;
	inline static ShaderItem* Outline;
};
