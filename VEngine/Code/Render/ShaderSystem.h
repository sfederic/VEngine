#pragma once

#include <string>
#include <vector>

class VertexShader;
class PixelShader;
class ShaderItem;

namespace ShaderSystem
{
	void Init();
	VertexShader* FindVertexShader(const std::wstring& filename);
	PixelShader* FindPixelShader(const std::wstring& filename);
	ShaderItem* FindShaderItem(const std::string& shaderItemName);
	std::vector<ShaderItem*> GetAllShaderItems();
	bool DoesShaderItemExist(std::string shaderItemName);
	void ClearShaders();
};
