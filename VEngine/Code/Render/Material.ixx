export module Render.Material;

#include <DirectXMath.h>
#include <d3d11.h>

import Core.VEnum;
import Core.UID;

export class Material
{
public:
	static void InitEnumValues();

private:
	MaterialShaderData materialShaderData;

public:
	VEnum rastStateValue;
	VEnum blendStateValue;

	VEnum audioTypeValue;

	VEnum shaderItemValue;

	TextureData defaultTextureData;
	TextureData secondaryTextureData;

private:
	Texture2D* defaultTexture = nullptr;
	Texture2D* secondaryTexture = nullptr;
	ShaderItem* shaderItem = nullptr;
	Sampler* sampler = nullptr;
	RastState* rastState = nullptr;
	BlendState* blendState = nullptr;

public:
	DirectX::XMFLOAT2 uvOffsetSpeed = DirectX::XMFLOAT2(0.f, 0.f);
	float uvRotationSpeed = 0.f;

private:
	UID uid = 0;

public:
	Material(std::string textureFilename_, std::string shaderItemName);

	virtual void Create();
	virtual void Destroy();

	virtual Properties GetProps();

	ID3D11VertexShader* GetVertexShader();
	ID3D11PixelShader* GetPixelShader();
	ID3D11InputLayout* GetInputLayout();

	auto GetUID() const { return uid; }
	void SetUID(UID uid_) { uid = uid_; }

	auto& GetBlendState() { return *blendState; }
	void SetBlendState(BlendState* state) { blendState = state; }

	auto& GetRastState() { return *rastState; }
	void SetRastState(RastState* state) { rastState = state; }

	auto& GetMaterialShaderData() { return materialShaderData; }

	void SetDefaultTexture(Texture2D* texture) { defaultTexture = texture; }
	void SetSecondaryTexture(Texture2D* texture) { secondaryTexture = texture; }

	auto& GetDefaultTexture() { return *defaultTexture; }
	auto& GetSecondaryTexture() { return *secondaryTexture; }

	auto& GetShaderItem() { return *shaderItem; }
	void SetShaderItem(ShaderItem* shader) { shaderItem = shader; }

	auto& GetSampler() { return *sampler; }
};
