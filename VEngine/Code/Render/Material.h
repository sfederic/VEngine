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

struct ID3D11VertexShader;
struct ID3D11PixelShader;

class Material
{
public:
	static void SetupBlendShaderItemsAndRastStateValues();

	MaterialShaderData materialShaderData;

	VEnum rastStateValue;
	VEnum blendStateValue;

	VEnum shaderItemValue;
	TextureData defaultTextureData;
	TextureData secondaryTextureData;

	Texture2D* defaultTexture = nullptr;
	Texture2D* secondaryTexture = nullptr;
	ShaderItem* shader = nullptr;
	Sampler* sampler = nullptr;
	RastState* rastState = nullptr;
	BlendState* blendState = nullptr;

	DirectX::XMFLOAT2 uvOffsetSpeed = DirectX::XMFLOAT2(0.f, 0.f);
	float uvRotationSpeed = 0.f;

	Material(std::string textureFilename_, std::string shaderItemName);

	virtual void Create();
	virtual void Destroy();

	virtual Properties GetProps();

	ID3D11VertexShader* GetVertexShader();
	ID3D11PixelShader* GetPixelShader();

	UID GetUID() const { return uid; }
	void SetUID(UID uid_) { uid = uid_; }

private:
	UID uid = 0;
};
