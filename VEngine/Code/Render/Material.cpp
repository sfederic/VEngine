#include "vpch.h"
#include "Material.h"
#include "RenderUtils.h"
#include "Renderer.h"
#include "ShaderSystem.h"
#include "TextureSystem.h"
#include "MaterialSystem.h"
#include "Texture2D.h"
#include "VString.h"
#include "WorldEditor.h"
#include "Actors/Actor.h"
#include "Components/MeshComponent.h"
#include "Log.h"
#include "Render/VertexShader.h"
#include "Render/PixelShader.h"

Material::Material(std::string textureFilename_, ShaderItemNames shaderItemNames)
{
	textureData.filename = textureFilename_;

	shaderData.vertexShaderFilename = VString::wstos(shaderItemNames.first);
	shaderData.pixelShaderFilename = VString::wstos(shaderItemNames.second);

	//@Todo: I don't like this. There needs to be a way to check if something inherits from VEnum when serialising
	rastStateValue.Add(RastStates::solid);
	rastStateValue.Add(RastStates::noBackCull);
	rastStateValue.Add(RastStates::shadow);
	rastStateValue.Add(RastStates::wireframe);

	blendStateValue.Add(BlendStates::null);
	blendStateValue.Add(BlendStates::Default);
}

void Material::Create()
{
	texture = textureSystem.FindTexture2D(textureData.filename);
	sampler = RenderUtils::GetDefaultSampler();
	rastState = Renderer::GetRastState(rastStateValue.GetValue());
	blendState = Renderer::GetBlendState(blendStateValue.GetValue());

	vertexShader = shaderSystem.FindVertexShader(VString::stows(shaderData.vertexShaderFilename));
	pixelShader = shaderSystem.FindPixelShader(VString::stows(shaderData.pixelShaderFilename));
}

void Material::Destroy()
{
	materialSystem.DestroyMaterial(uid);
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
	auto meshes = WorldEditor::GetPickedActor()->GetComponentsOfType<MeshComponent>();
	for (auto mesh : meshes)
	{
		mesh->material->texture = swapTexture;
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
			mesh->material->rastState = foundRastState;
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
			mesh->material->blendState = foundBlendState;
		}
	}
}

static void ReassignShader(void* data)
{
	//auto shaderData = (ShaderData*)data;
	//ShaderItem* foundShader = shaderSystem.FindShader(VString::stows(shaderData->filename));
	//if (foundShader == nullptr)
	//{
	//	Log("%s not found on shader change.", shaderData->filename);
	//	return;
	//}

	//auto meshes = WorldEditor::GetPickedActor()->GetComponentsOfType<MeshComponent>();
	//for (auto mesh : meshes)
	//{
	//	mesh->material->shader = foundShader;
	//}
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
