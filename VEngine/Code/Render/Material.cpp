#include "vpch.h"
#include "Material.h"
#include "RenderUtils.h"
#include "Renderer.h"
#include "ShaderSystem.h"
#include "TextureSystem.h"
#include "MaterialSystem.h"
#include "Texture2D.h"
#include "Core/VString.h"
#include "Core/WorldEditor.h"
#include "Actors/Actor.h"
#include "Components/MeshComponent.h"
#include "Core/Log.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ShaderItem.h"
#include "RastStates.h"
#include "BlendStates.h"

VEnum rastStates;
VEnum blendStates;

void Material::SetupBlendAndRastStateValues()
{
	rastStates.Add(RastStates::solid);
	rastStates.Add(RastStates::noBackCull);
	rastStates.Add(RastStates::shadow);
	rastStates.Add(RastStates::wireframe);

	blendStates.Add(BlendStates::null);
	blendStates.Add(BlendStates::Default);
}

Material::Material(std::string textureFilename_, ShaderItem* shaderItem)
{
	textureData.filename = textureFilename_;

	shaderData.shaderItemName = shaderItem->GetName();

	rastStateValue = rastStates;
	blendStateValue = blendStates;
}

void Material::Create()
{
	texture = TextureSystem::FindTexture2D(textureData.filename);
	sampler = RenderUtils::GetDefaultSampler();
	rastState = Renderer::GetRastState(rastStateValue.GetValue());
	blendState = Renderer::GetBlendState(blendStateValue.GetValue());
	shader = ShaderSystem::FindShaderItem(shaderData.shaderItemName);
}

void Material::Destroy()
{
	MaterialSystem::DestroyMaterial(uid);
}

static void ReassignTexture(void* data)
{
	auto textureData = (TextureData*)data;

	auto swapTexture = TextureSystem::FindTexture2D(textureData->filename);
	if (swapTexture == nullptr)
	{
		Log("%s wasn't found on texture change.", textureData->filename);
		return;
	}

	//@Todo: these 'get all mesh' calls aren't really right as each mesh component would
	//have its own properties, but it works ok for simple actors for now.
	//Could do WorldEditor::GetPickedComponent() here maybe.
	auto meshes = WorldEditor::GetPickedActor()->GetComponentsOfType<MeshComponent>();
	for (auto mesh : meshes)
	{
		mesh->GetMaterial().texture = swapTexture;
	}
}

static void ReassignRastState(void* data)
{
	auto rastState = (VEnum*)data;
	auto rastName = rastState->GetValue();
	RastState* foundRastState = Renderer::GetRastState(rastName);
	if (foundRastState == nullptr)
	{
		Log("[%s] not found on rast state change.", rastName.c_str());
		return;
	}

	if (WorldEditor::GetPickedActor())
	{
		auto meshes = WorldEditor::GetPickedActor()->GetComponentsOfType<MeshComponent>();
		for (auto mesh : meshes)
		{
			mesh->GetMaterial().rastState = foundRastState;
		}
	}
}

static void ReassignBlendState(void* data)
{
	auto blendState = (VEnum*)data;
	auto blendName = blendState->GetValue();
	BlendState* foundBlendState = Renderer::GetBlendState(blendName);
	if (foundBlendState == nullptr)
	{
		Log("[%s] not found on blend state change.", blendName.c_str());
		return;
	}

	if (WorldEditor::GetPickedActor())
	{
		auto meshes = WorldEditor::GetPickedActor()->GetComponentsOfType<MeshComponent>();
		for (auto mesh : meshes)
		{
			mesh->GetMaterial().blendState = foundBlendState;
		}
	}
}

static void ReassignShader(void* data)
{
	auto shaderData = (ShaderData*)data;
	auto foundShader = ShaderSystem::FindShaderItem(shaderData->shaderItemName);
	if (foundShader == nullptr)
	{
		Log("%s not found on shader change.", shaderData->shaderItemName.c_str());
		return;
	}

	auto meshes = WorldEditor::GetPickedActor()->GetComponentsOfType<MeshComponent>();
	for (auto mesh : meshes)
	{
		mesh->GetMaterial().shader = foundShader;
	}
}

Properties Material::GetProps()
{
	Properties props("Material");

	//Prop names using 'M_' notation here becauase they merge with MeshComponent props
	props.Add("M_Texture", &textureData).change = ReassignTexture;
	props.Add("M_Use Texture", &materialShaderData.useTexture);
	props.Add("M_Shader", &shaderData).change = ReassignShader;
	props.Add("M_Rast State", &rastStateValue).change = ReassignRastState;
	props.Add("M_Blend State", &blendStateValue).change = ReassignBlendState;
	props.Add("M_UvOffset", &materialShaderData.uvOffset);
	props.Add("M_UvOffsetSpeed", &uvOffsetSpeed);
	props.Add("M_UvScale", &materialShaderData.uvScale);
	props.Add("M_UvRotation", &materialShaderData.uvRotation);
	props.Add("M_UvRotationSpeed", &uvRotationSpeed);
	props.Add("M_Ambient", &materialShaderData.ambient);
	props.Add("M_Specular", &materialShaderData.specular);
	props.Add("M_Smoothness", &materialShaderData.smoothness);
	props.Add("M_Metallic", &materialShaderData.metallic);
	return props;
}

ID3D11VertexShader* Material::GetVertexShader()
{
	return shader->GetVertexShader();
}

ID3D11PixelShader* Material::GetPixelShader()
{
	return shader->GetPixelShader();
}
