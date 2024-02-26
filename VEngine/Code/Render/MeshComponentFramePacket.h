#pragma once

#include "Render/RenderPropertyStructs.h"
#include "Render/ShaderData/MaterialShaderData.h"
#include "Render/PipelineObjects.h"
#include "Render/Texture2D.h"
#include "Render/ShaderItem.h"
#include <DirectXMath.h>

struct MeshComponentFramePacket
{
	DirectX::XMMATRIX worldMatrix;
	Buffer vertexBuffer;
	Texture2D texture;
	ShaderItem shader;
	Sampler sampler;
	RastState rastState;
	BlendState blendState;
	TextureData textureData;
	MaterialShaderData materialShaderData;
	DirectX::XMFLOAT2 uvOffsetSpeed = DirectX::XMFLOAT2(0.f, 0.f);
	float uvRotationSpeed = 0.f;
	uint32_t vertexCount = 0;
};
