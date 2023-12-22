#include "vpch.h"
#include "ShaderItem.h"
#include <cassert>
#include "VertexShader.h"
#include "PixelShader.h"
#include "ShaderSystem.h"

ShaderItem::ShaderItem(std::string shaderItemName_,
	std::wstring vertexShaderFilename_,
	std::wstring pixelShaderFilename_) :
	shaderItemName(shaderItemName_),
	pixelShaderFilename(pixelShaderFilename_),
	vertexShaderFilename(vertexShaderFilename_)
{
	ShaderSystem::AddShaderItem(this);

	vertexShader = ShaderSystem::FindVertexShader(vertexShaderFilename)->GetShader();
	pixelShader = ShaderSystem::FindPixelShader(pixelShaderFilename)->GetShader();
}
