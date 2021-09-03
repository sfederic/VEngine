#include "Material.h"
#include "MaterialSystem.h"
#include "Render/Renderer.h"
#include "Render/ShaderSystem.h"

Material::Material(std::wstring textureFilename_, std::wstring shaderFilename_)
{
	textureFilename = textureFilename_;
	shaderFilename = shaderFilename_;

	materialSystem.AddMaterial(this);
}

void Material::Create()
{
	texture = renderer.CreateTexture(textureFilename);
	sampler = renderer.CreateSampler();
	shader = shaderSystem.shaderMap.find(shaderFilename)->second;
	rastState = renderer.rastStateMap["solid"];
}
