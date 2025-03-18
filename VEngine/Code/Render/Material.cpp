#include <d3d11.h>

import Render.Material;
import Actors.Actor;
import Core.VEnum;
import Core.Property;
import Core.Properties;
import Core.World;
import Core.WorldEditor;
import Components.MeshComponent;
import Render.BlendStates;
import Render.RastStates;
import Render.TextureSystem;
import Render.Renderer;
import Render.ShaderSystem;
import Render.RenderPropertyStructs;

static VEnum rastStates;
static VEnum blendStates;
static VEnum shaderItemNames;
static VEnum materialAudioTypes;

void Material::InitEnumValues()
{
	rastStates.Add(RastStates::solid);
	rastStates.Add(RastStates::noBackCull);
	rastStates.Add(RastStates::shadow);
	rastStates.Add(RastStates::wireframe);
	rastStates.Add(RastStates::frontCull);

	blendStates.Add(BlendStates::Default);
	blendStates.Add(BlendStates::Transparent);

	for (const auto shaderItem : ShaderSystem::GetAllShaderItems())
	{
		shaderItemNames.Add(shaderItem->GetName());
	}

	materialAudioTypes.Add(MaterialAudioType::Default);
	materialAudioTypes.Add(MaterialAudioType::Grass);
	materialAudioTypes.Add(MaterialAudioType::Metal);
	materialAudioTypes.Add(MaterialAudioType::Wood);
}

Material::Material(std::string textureFilename_, std::string shaderItemName)
{
	defaultTextureData.filename = textureFilename_;

	rastStateValue = rastStates;
	blendStateValue = blendStates;

	audioTypeValue = materialAudioTypes;

	shaderItemValue = shaderItemNames;
	shaderItemValue.SetValue(shaderItemName);
}

void Material::Create()
{
	defaultTexture = TextureSystem::FindTexture2D(defaultTextureData.filename);
	secondaryTexture = TextureSystem::FindTexture2D(secondaryTextureData.filename);

	sampler = &Renderer::Get().GetDefaultSampler();
	rastState = Renderer::Get().GetRastState(rastStateValue.GetValue());
	blendState = Renderer::Get().GetBlendState(blendStateValue.GetValue());
	shaderItem = ShaderSystem::FindShaderItem(shaderItemValue.GetValue());
}

void Material::Destroy()
{
	MaterialSystem::DestroyMaterial(uid);
}

static void ReassignTexture(Property& prop)
{
	auto textureData = prop.GetData<TextureData>();

	auto swapTexture = TextureSystem::FindTexture2D(textureData->filename);
	if (swapTexture == nullptr)
	{
		Log("%s wasn't found on texture change.", textureData->filename.c_str());
		return;
	}

	auto actor = World::Get().GetActorByUIDAllowNull(prop.ownerUID);
	if (actor)
	{
		auto meshes = actor->GetComponents<MeshComponent>();
		for (auto mesh : meshes)
		{
			mesh->GetMaterial().SetDefaultTexture(swapTexture);
		}
	}
}

static void ReassignTextureSecondary(Property& prop)
{
	auto textureData = prop.GetData<TextureData>();

	auto swapTexture = TextureSystem::FindTexture2D(textureData->filename);
	if (swapTexture == nullptr)
	{
		Log("%s wasn't found on texture change.", textureData->filename.c_str());
		return;
	}

	auto actor = World::Get().GetActorByUIDAllowNull(prop.ownerUID);
	if (actor)
	{
		auto meshes = actor->GetComponents<MeshComponent>();
		for (auto mesh : meshes)
		{
			mesh->GetMaterial().SetSecondaryTexture(swapTexture);
		}
	}
}

static void ReassignRastState(Property& prop)
{
	auto rastState = prop.GetData<VEnum>();
	auto rastName = rastState->GetValue();
	RastState* foundRastState = Renderer::Get().GetRastState(rastName);
	if (foundRastState == nullptr)
	{
		Log("[%s] not found on rast state change.", rastName.c_str());
		return;
	}

	if (WorldEditor::GetPickedActor())
	{
		auto meshes = WorldEditor::GetPickedActor()->GetComponents<MeshComponent>();
		for (auto mesh : meshes)
		{
			mesh->GetMaterial().SetRastState(foundRastState);
		}
	}
}

static void ReassignBlendState(Property& prop)
{
	auto blendState = prop.GetData<VEnum>();
	auto blendName = blendState->GetValue();
	BlendState* foundBlendState = Renderer::Get().GetBlendState(blendName);
	if (foundBlendState == nullptr)
	{
		Log("[%s] not found on blend state change.", blendName.c_str());
		return;
	}

	if (WorldEditor::GetPickedActor())
	{
		auto meshes = WorldEditor::GetPickedActor()->GetComponents<MeshComponent>();
		for (auto mesh : meshes)
		{
			mesh->GetMaterial().SetBlendState(foundBlendState);
		}
	}
}

static void ReassignShader(Property& prop)
{
	auto shaderData = prop.GetData<VEnum>();
	auto foundShader = ShaderSystem::FindShaderItem(shaderData->GetValue());
	if (foundShader == nullptr)
	{
		Log("[%s] shader item not found on shader change.", shaderData->GetValue().c_str());
		return;
	}

	auto meshes = WorldEditor::GetPickedActor()->GetComponents<MeshComponent>();
	for (auto mesh : meshes)
	{
		mesh->GetMaterial().SetShaderItem(foundShader);
	}
}

Properties Material::GetProps()
{
	Properties props("Material");

	//Prop names are using 'M_' notation here because they merge with MeshComponent props
	//and need them sorted by key (alphabetically) via std::map.

	props.Add("M_Texture", &defaultTextureData).change = ReassignTexture;
	props.Add("M_Texture2", &secondaryTextureData).change = ReassignTextureSecondary;

	props.Add("M_Use Texture", &materialShaderData.useTexture);
	props.Add("M_Shader", &shaderItemValue).change = ReassignShader;
	props.Add("M_Rast State", &rastStateValue).change = ReassignRastState;
	props.Add("M_Blend State", &blendStateValue).change = ReassignBlendState;
	props.Add("M_AudioType", &audioTypeValue);
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
	return shaderItem->GetVertexShader();
}

ID3D11PixelShader* Material::GetPixelShader()
{
	return shaderItem->GetPixelShader();
}

ID3D11InputLayout* Material::GetInputLayout()
{
	return shaderItem->GetInputLayout();
}
