#pragma once

#include "Core/Properties.h"
#include "Core/VEnum.h"
#include "Render/ShaderData/MaterialShaderData.h"

class Texture2D;
class Sampler;
class RastState;
class BlendState;
class VertexShader;
class PixelShader;
class ShaderItem;

struct ID3D11VertexShader;
struct ID3D11PixelShader;

//TODO: Multiple materials per mesh: I think the best way of doing this is to have an array of materials in the 
//shader, while assigning an index value pertaining to each material per vertex. It's pretty costly and difficult
//to set up though, I don't think simpler games would need it yet.

class Material
{
public:
	static void InitEnumValues();

	MaterialShaderData materialShaderData;

	VEnum rastStateValue;
	VEnum blendStateValue;

	VEnum audioTypeValue;

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
	ID3D11InputLayout* GetInputLayout();

	UID GetUID() const { return uid; }
	void SetUID(UID uid_) { uid = uid_; }

private:
	UID uid = 0;
};
