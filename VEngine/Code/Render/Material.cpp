#include "Material.h"
#include "Render/RenderUtils.h"
#include "Render/Renderer.h"
#include "Render/ShaderSystem.h"
#include "Render/TextureSystem.h"
#include "Render/MaterialSystem.h"

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

Properties Material::GetProps()
{
	Properties props("Material");
	props.Add("UvOffset", &shaderData.uvOffset);
	props.Add("UvScale", &shaderData.uvScale);
	props.Add("UvRotation", &shaderData.uvRotation);
	return props;
}
