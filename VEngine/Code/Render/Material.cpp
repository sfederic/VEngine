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
	shaderData.filename = shaderFilename_;

	materialSystem.AddMaterial(this);
}

void Material::Create()
{
	texture = textureSystem.FindTexture2D(textureData.filename);
	sampler = RenderUtils::GetDefaultSampler();
	shader = shaderSystem.FindShader(stows(shaderData.filename));
	rastState = renderer.rastStateMap[rastStateName];
	blendState = renderer.blendStateMap["default"];
}

static void ReassignTexture(void* data)
{
	auto textureData = (TextureData*)data;

	Texture2D* swapTexture = textureSystem.FindTexture2D(textureData->filename);
	if (swapTexture == nullptr)
	{
		editor->Log("%s wasn't found on texture change.", textureData->filename);
		return;
	}

	//TODO: these 'get all mesh' calls aren't really right as each mesh component would
	//have its own properties, but it works ok for simple actors for now.
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
	if (foundRastState == nullptr)
	{
		editor->Log("%s not found on rast state change.", *rastName);
		return;
	}

	auto meshes = worldEditor.pickedActor->GetComponentsOfType<MeshComponent>();
	for (auto mesh : meshes)
	{
		mesh->material->rastState = foundRastState;
	}
}

static void ReassignShader(void* data)
{
	auto shaderName = (std::string*)data;
	ShaderItem* foundShader = shaderSystem.FindShader(stows(*shaderName));
	if (foundShader == nullptr)
	{
		editor->Log("%s not found on shader change.", *shaderName);
		return;
	}

	auto meshes = worldEditor.pickedActor->GetComponentsOfType<MeshComponent>();
	for (auto mesh : meshes)
	{
		mesh->material->shader = foundShader;
	}
}

Properties Material::GetProps()
{
	Properties props("Material");
	props.Add("Texture", &textureData).change = ReassignTexture;
	props.Add("Shader", &shaderData).change = ReassignShader;
	props.Add("Rast State", &rastStateName).change = ReassignRastState;
	props.Add("UvOffset", &materialShaderData.uvOffset);
	props.Add("UvScale", &materialShaderData.uvScale);
	props.Add("UvRotation", &materialShaderData.uvRotation);
	return props;
}
