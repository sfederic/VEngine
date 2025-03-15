import Render.ShaderItem;
import Render.VertexShader;
import Render.PixelShader;

import <string>;

ShaderItem::ShaderItem(std::string shaderItemName_,
	std::wstring vertexShaderFilename_,
	std::wstring pixelShaderFilename_) :
	shaderItemName(shaderItemName_),
	pixelShaderFilename(pixelShaderFilename_),
	vertexShaderFilename(vertexShaderFilename_)
{
	vertexShader = ShaderSystem::FindVertexShader(vertexShaderFilename)->GetShader();
	pixelShader = ShaderSystem::FindPixelShader(pixelShaderFilename)->GetShader();
}

ID3D11InputLayout* ShaderItem::GetInputLayout() const
{
	auto vertShader = ShaderSystem::FindVertexShader(vertexShaderFilename);
	return vertShader->GetInputLayout();
}
