#include "Material.h"
#include "Render/RenderUtils.h"
#include "Render/Renderer.h"
#include "Render/ShaderSystem.h"
#include "Render/TextureSystem.h"
#include "Render/MaterialSystem.h"
#include "VString.h"
#include "WorldEditor.h"
#include "Actors/Actor.h"
#include "Components/MeshComponent.h"
#include "Editor/Editor.h"

Material::Material()
{
}

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
	blendState = renderer.blendStateMap["default"];
}

void ReassignTexture(void* data)
{
	std::string* str = (std::string*)data;

	Texture2D* swapTexture = textureSystem.FindTexture2D(stows(*str));
	if (swapTexture == nullptr)
	{
		editor->Log("Texture wasn't found on change.");
		return;
	}

	auto meshes = worldEditor.pickedActor->GetComponentsOfType<MeshComponent>();
	for (auto mesh : meshes)
	{
		mesh->material->texture = swapTexture;
	}
}

Properties Material::GetProps()
{
	Properties props("Material");
	props.Add("Texture", &textureFilename).change = ReassignTexture;
	props.Add("Shader", &shaderFilename);
	props.Add("UvOffset", &shaderData.uvOffset);
	props.Add("UvScale", &shaderData.uvScale);
	props.Add("UvRotation", &shaderData.uvRotation);
	return props;
}
