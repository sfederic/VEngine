#include "Material.h"
#include "Render/RenderUtils.h"
#include "Render/Renderer.h"
#include "Render/ShaderSystem.h"
#include "Render/TextureSystem.h"
#include "Render/MaterialSystem.h"
#include "VString.h"

Material::Material(std::string textureFilename_, std::string shaderFilename_)
{
	textureFilename = textureFilename_;
	shaderFilename = shaderFilename_;

	materialSystem.AddMaterial(this);
}

void Material::Create()
{
	texture = textureSystem.FindTexture2D(stows(textureFilename));
	sampler = RenderUtils::GetDefaultSampler();
	shader = shaderSystem.FindShader(stows(shaderFilename));
	rastState = renderer.rastStateMap["solid"];
}

Properties Material::GetProps()
{
	Properties props("Material");
	props.Add("Texture", &textureFilename);
	props.Add("Shader", &shaderFilename);
	props.Add("UvOffset", &shaderData.uvOffset);
	props.Add("UvScale", &shaderData.uvScale);
	props.Add("UvRotation", &shaderData.uvRotation);
	return props;
}
