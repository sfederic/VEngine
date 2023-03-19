#pragma once

#include <string>

class VertexShader;
class PixelShader;
class ShaderItem;

//@Todo: shader hotreload was taken out of the engine after moving away from using D3DCompileFromFile() at runtime.
//Could probably be brought back, but for now it's not too heavy to close the program, compile the shader and re-run.
//If engine startup times got worse (which runtime shader compiles contributed to heavily), then bring it back.

namespace ShaderSystem
{
	void Init();

	void AddShaderItem(ShaderItem* shaderItem);

	VertexShader* FindVertexShader(const std::wstring filename);
	PixelShader* FindPixelShader(const std::wstring filename);
	ShaderItem* FindShaderItem(const std::string shaderItemName);

	bool DoesShaderItemExist(std::string shaderItemName);

	void ClearShaders();
};
