#pragma once

#include "Core/Properties.h"
#include "Core/VEnum.h"
#include "Render/ShaderData/MaterialShaderData.h"

class Texture2D;
struct Sampler;
struct RastState;
struct BlendState;
class VertexShader;
class PixelShader;
class ShaderItem;

class Material
{
private:
	UID uid = 0;

public:
	Texture2D* texture = nullptr;
	ShaderItem* shader = nullptr;
	Sampler* sampler = nullptr;
	RastState* rastState = nullptr;
	BlendState* blendState = nullptr;

	TextureData textureData;
	ShaderData shaderData;

	DirectX::XMFLOAT2 uvOffsetSpeed = DirectX::XMFLOAT2(0.f, 0.f);
	float uvRotationSpeed = 0.f;

	MaterialShaderData materialShaderData;

	VEnum rastStateValue;
	VEnum blendStateValue;

	Material() {}
	Material(std::string textureFilename_, ShaderItem* shaderItem);

	virtual void Create();
	virtual void Destroy();

	virtual Properties GetProps();

	ID3D11VertexShader* GetVertexShader();
	ID3D11PixelShader* GetPixelShader();

	UID GetUID() { return uid; }
	void SetUID(UID uid_) { uid = uid_; }

	static void SetupBlendAndRastStateValues();
};
