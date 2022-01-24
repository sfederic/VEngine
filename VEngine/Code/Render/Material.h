#pragma once
#include <DirectXMath.h>
#include <string>
#include "Properties.h"
#include "RenderTypes.h"
#include "UID.h"

using namespace DirectX;

struct Texture2D;
struct Sampler;
struct RastState;
struct ShaderItem;
struct BlendState;

//The data passed into a shader's constant buffer. Has to be seperate because of byte packing.
struct MaterialShaderData
{
	XMFLOAT4 ambient = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	XMFLOAT4 emissive = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	XMFLOAT4 diffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	XMFLOAT4 specular = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	XMFLOAT2 uvOffset = XMFLOAT2(0.f, 0.f);
	XMFLOAT2 uvScale = XMFLOAT2(1.f, 1.f);
	float uvRotation = 0.f;
	float specularPower = 0.f;
	int useTexture; //Keep in mind that bools in HLSL are 4 bytes

private:
	float padding[1];
};

struct Material
{
	UID uid = GenerateUID();

	XMFLOAT2 uvOffsetSpeed = XMFLOAT2(0.f, 0.f);
	float uvRotationSpeed = 0.f;

	MaterialShaderData materialShaderData;

	Texture2D* texture = nullptr;
	Sampler* sampler = nullptr;
	RastState* rastState = nullptr;
	ShaderItem* shader = nullptr;
	BlendState* blendState = nullptr;

	std::string name;

	TextureData textureData;
	ShaderData shaderData;

	std::string rastStateName = RastStateNames::solid;

	Material();
	Material(std::string textureFilename_, std::string shaderFilename_);
	virtual void Create();
	virtual void Destroy();
	virtual Properties GetProps();
};
