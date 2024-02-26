#pragma once

#include "Render/RenderPropertyStructs.h"
#include "Render/ShaderData/MaterialShaderData.h"
#include <DirectXMath.h>

class Buffer;
class RastState;
class Texture2D;
class ShaderItem;
class Sampler;
class BlendState;

struct MeshComponentFramePacket
{
	Buffer* vertexBuffer = nullptr;

	RastState* rastState = nullptr;
	Texture2D* texture = nullptr;
	ShaderItem* shader = nullptr;
	Sampler* sampler = nullptr;
	RastState* rastState = nullptr;
	BlendState* blendState = nullptr;

	TextureData textureData;

	MaterialShaderData materialShaderData;

	DirectX::XMFLOAT2 uvOffsetSpeed = DirectX::XMFLOAT2(0.f, 0.f);
	float uvRotationSpeed = 0.f;
};
