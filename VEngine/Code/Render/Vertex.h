#pragma once

#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT4 colour = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.f);
	DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 normal = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 tangent = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT2 uv = DirectX::XMFLOAT2(0.f, 0.f);
	uint32_t boneIndices[4]{};
	float weights[3]{};
};
