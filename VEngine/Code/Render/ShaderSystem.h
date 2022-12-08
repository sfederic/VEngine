#pragma once

#include <string>

class VertexShader;
class PixelShader;
class ShaderItem;

namespace ShaderSystem
{
	void Init();
	void Tick();
	VertexShader* FindVertexShader(const std::wstring filename);
	PixelShader* FindPixelShader(const std::wstring filename);
	void AddShaderItem(ShaderItem* shaderItem);
	ShaderItem* FindShaderItem(const std::string shaderItemName);
	void ClearShaders();
};
