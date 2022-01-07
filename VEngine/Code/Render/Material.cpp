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
#include "Log.h"

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
	shader = shaderSystem.FindShader(VString::stows(shaderData.filename));
	rastState = renderer.rastStateMap[rastStateName];
}

void Material::Destroy()
{
	materialSystem.materials.erase(uid);
}

static void ReassignTexture(void* data)
{
	auto textureData = (TextureData*)data;

	Texture2D* swapTexture = textureSystem.FindTexture2D(textureData->filename);
	if (swapTexture == nullptr)
	{
		Log("%s wasn't found on texture change.", textureData->filename);
		return;
	}

	//@Todo: these 'get all mesh' calls aren't really right as each mesh component would
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
		Log("%s not found on rast state change.", rastName->c_str());
		return;
	}

	if (worldEditor.pickedActor)
	{
		auto meshes = worldEditor.pickedActor->GetComponentsOfType<MeshComponent>();
		for (auto mesh : meshes)
		{
			mesh->material->rastState = foundRastState;
		}
	}
}

static void ReassignShader(void* data)
{
	auto shaderData = (ShaderData*)data;
	ShaderItem* foundShader = shaderSystem.FindShader(VString::stows(shaderData->filename));
	if (foundShader == nullptr)
	{
		Log("%s not found on shader change.", shaderData->filename);
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
	props.Add("UvOffsetSpeed", &uvOffsetSpeed);
	props.Add("UvScale", &materialShaderData.uvScale);
	props.Add("UvRotation", &materialShaderData.uvRotation);
	props.Add("UvRotationSpeed", &uvRotationSpeed);
	return props;
}
