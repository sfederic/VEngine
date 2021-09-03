#include "Material.h"
#include "MaterialSystem.h"
#include "Render/RenderUtils.h"
#include "Render/Renderer.h"
#include "Render/ShaderSystem.h"
#include "Render/TextureSystem.h"

Material::Material(std::wstring textureFilename_, std::wstring shaderFilename_)
{
	textureFilename = textureFilename_;
	shaderFilename = shaderFilename_;

	materialSystem.AddMaterial(this);
}

void Material::Create()
{
	texture = textureSystem.FindTexture2D(textureFilename);
	sampler = RenderUtils::GetDefaultSampler();
	shader = shaderSystem.FindShader(shaderFilename);
	rastState = renderer.rastStateMap["solid"];
}
