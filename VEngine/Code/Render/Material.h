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
private:
	UID uid = 0;

public:
	static void SetupBlendShaderItemsAndRastStateValues();

	Texture2D* defaultTexture = nullptr;
	Texture2D* secondaryTexture = nullptr;
	ShaderItem* shader = nullptr;
	Sampler* sampler = nullptr;
	RastState* rastState = nullptr;
	BlendState* blendState = nullptr;

	TextureData defaultTextureData;
	TextureData secondaryTextureData;
	VEnum shaderItemValue;

	DirectX::XMFLOAT2 uvOffsetSpeed = DirectX::XMFLOAT2(0.f, 0.f);
	float uvRotationSpeed = 0.f;

	MaterialShaderData materialShaderData;

	VEnum rastStateValue;
	VEnum blendStateValue;

	Material(std::string textureFilename_, ShaderItem* shaderItem);

	virtual void Create();
	virtual void Destroy();

	virtual Properties GetProps();

	ID3D11VertexShader* GetVertexShader();
	ID3D11PixelShader* GetPixelShader();

	UID GetUID() const { return uid; }
	void SetUID(UID uid_) { uid = uid_; }
};
