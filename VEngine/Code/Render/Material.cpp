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
	materialSystem.AddMaterial(this);
}

Material::Material(std::string textureFilename_, std::string shaderFilename_)
{
	textureData.filename = textureFilename_;
	shaderFilename = shaderFilename_;

	materialSystem.AddMaterial(this);
}

void Material::Create()
{
	texture = textureSystem.FindTexture2D(textureData.filename);
	sampler = RenderUtils::GetDefaultSampler();
	shader = shaderSystem.FindShader(stows(shaderFilename));
	rastState = renderer.rastStateMap[rastStateName];
	blendState = renderer.blendStateMap["default"];
}

static void ReassignTexture(void* data)
{
	auto textureData = (TextureData*)data;

	Texture2D* swapTexture = textureSystem.FindTexture2D(textureData->filename);
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

static void ReassignRastState(void* data)
{
	auto rastName = (std::string*)data;
	RastState* foundRastState = renderer.rastStateMap[*rastName];

	auto meshes = worldEditor.pickedActor->GetComponentsOfType<MeshComponent>();
	for (auto mesh : meshes)
	{
		mesh->material->rastState = foundRastState;
	}
}

Properties Material::GetProps()
{
	Properties props("Material");
	props.Add("Texture", &textureData).change = ReassignTexture;
	props.Add("Shader", &shaderFilename);
	props.Add("Rast State", &rastStateName).change = ReassignRastState;
	props.Add("UvOffset", &shaderData.uvOffset);
	props.Add("UvScale", &shaderData.uvScale);
	props.Add("UvRotation", &shaderData.uvRotation);
	return props;
}
