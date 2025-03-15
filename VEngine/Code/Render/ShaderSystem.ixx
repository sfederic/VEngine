export module Render.ShaderSystem;

import <string>;
import <vector>;

export class VertexShader;
export class PixelShader;
export class ShaderItem;

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
