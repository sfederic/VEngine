#pragma once

#include <string>

class VertexShader;
class PixelShader;
class ShaderItem;

namespace ShaderSystem
{
	void Init();

	void AddShaderItem(ShaderItem* shaderItem);

	VertexShader* FindVertexShader(const std::wstring filename);
	PixelShader* FindPixelShader(const std::wstring filename);
	ShaderItem* FindShaderItem(const std::string shaderItemName);

	void ClearShaders();
};
