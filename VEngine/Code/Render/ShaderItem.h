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
	ShaderItem(const std::string shaderItemName_,
		const std::wstring vertexShaderFilename_,
		const std::wstring pixelShaderFilename_);

	ID3D11VertexShader* GetVertexShader();
	ID3D11PixelShader* GetPixelShader();

	std::string GetName() { return shaderItemName; }

	std::wstring GetVertexShaderFilename() { return vertexShaderFilename; }
	std::wstring GetPixelShaderFilename() { return pixelShaderFilename; }

	UID GetUID() { return uid; }

private:
	//UID here is used for sorting meshes on render.
	UID uid = GenerateUID();

	std::string shaderItemName;

	std::wstring vertexShaderFilename;
	std::wstring pixelShaderFilename;
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
};
