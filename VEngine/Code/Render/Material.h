#pragma once
#include <DirectXMath.h>
#include <string>

using namespace DirectX;

struct Texture2D;
struct Sampler;
struct RastState;
struct ShaderItem;

//The data passed into a shader's constant buffer. Has to be seperate because of byte packing.
struct MaterialShaderData
{
	XMFLOAT4 ambient = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	XMFLOAT2 uvOffset = XMFLOAT2(0.f, 0.f);
	XMFLOAT2 uvScale = XMFLOAT2(1.f, 1.f);
	float uvRotation = 0.f;

private:
	float padding[3];
};

struct Material
{
	MaterialShaderData shaderData;

	Texture2D* texture = nullptr;
	Sampler* sampler = nullptr;
	RastState* rastState = nullptr;
	ShaderItem* shader = nullptr;

	std::wstring textureFilename;
	std::wstring shaderFilename;

	Material(std::wstring textureFilename_, std::wstring shaderFilename_);
	virtual void Create();
};
