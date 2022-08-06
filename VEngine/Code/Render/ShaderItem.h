#pragma once

#include <string>

class VertexShader;
class PixelShader;
struct ID3D11VertexShader;
struct ID3D11PixelShader;

//Helper structure when finding shaders from Shader System
class ShaderItem
{
public:
	//ShaderItem(VertexShader* vs, PixelShader* ps) : vertexShader(vs), pixelShader(ps) {}
	ShaderItem() {}
	ShaderItem(const std::wstring vertexShaderName, const std::wstring pixelShaderName);

	ID3D11VertexShader* GetVertexShader();
	ID3D11PixelShader* GetPixelShader();

	std::wstring GetVertexShaderFilename() { return vertexShaderFilename; }
	std::wstring GetPixelShaderFilename() { return pixelShaderFilename; }

private:
	VertexShader* vertexShader = nullptr;
	PixelShader* pixelShader = nullptr;

	std::wstring vertexShaderFilename;
	std::wstring pixelShaderFilename;
};

struct ShaderItems
{
	inline static ShaderItem Default;
	inline static ShaderItem DefaultClip;
	inline static ShaderItem Unlit;
	inline static ShaderItem Animation;
	inline static ShaderItem Shadow;
	inline static ShaderItem Instance;
	inline static ShaderItem SolidColour;
	inline static ShaderItem UI;
	inline static ShaderItem PostProcess;
};
